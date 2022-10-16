// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if !V8_ENABLE_WEBASSEMBLY
#error This header should only be included if WebAssembly is enabled.
#endif  // !V8_ENABLE_WEBASSEMBLY

#ifndef V8_WASM_WASM_OBJECTS_INL_H_
#define V8_WASM_WASM_OBJECTS_INL_H_

#include <type_traits>

#include "src/base/memory.h"
#include "src/heap/heap-write-barrier-inl.h"
#include "src/objects/contexts-inl.h"
#include "src/objects/foreign.h"
#include "src/objects/heap-number.h"
#include "src/objects/js-array-buffer-inl.h"
#include "src/objects/js-function-inl.h"
#include "src/objects/js-objects-inl.h"
#include "src/objects/managed.h"
#include "src/objects/oddball-inl.h"
#include "src/objects/script-inl.h"
#include "src/roots/roots.h"
#include "src/wasm/wasm-code-manager.h"
#include "src/wasm/wasm-module.h"
#include "src/wasm/wasm-objects.h"

// Has to be the last include (doesn't have include guards)
#include "src/objects/object-macros.h"

namespace v8 {
namespace internal {

#include "torque-generated/src/wasm/wasm-objects-tq-inl.inc"

TQ_OBJECT_CONSTRUCTORS_IMPL(WasmTagObject)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmExceptionTag)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmCapiFunctionData)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmExportedFunctionData)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmGlobalObject)
OBJECT_CONSTRUCTORS_IMPL(WasmInstanceObject, JSObject)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmObject)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmMemoryObject)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmModuleObject)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmTableObject)
TQ_OBJECT_CONSTRUCTORS_IMPL(AsmWasmData)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmFunctionData)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmTypeInfo)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmStruct)
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmArray)

CAST_ACCESSOR(WasmInstanceObject)

#define OPTIONAL_ACCESSORS(holder, name, type, offset)                  \
  DEF_GETTER(holder, has_##name, bool) {                                \
    Object value = TaggedField<Object, offset>::load(cage_base, *this); \
    return !value.IsUndefined(GetReadOnlyRoots(cage_base));             \
  }                                                                     \
  ACCESSORS_CHECKED2(holder, name, type, offset,                        \
                     !value.IsUndefined(GetReadOnlyRoots(cage_base)), true)

#define PRIMITIVE_ACCESSORS(holder, name, type, offset)                       \
  type holder::name() const {                                                 \
    if (COMPRESS_POINTERS_BOOL && alignof(type) > kTaggedSize) {              \
      /* TODO(ishell, v8:8875): When pointer compression is enabled 8-byte */ \
      /* size fields (external pointers, doubles and BigInt data) are only */ \
      /* kTaggedSize aligned so we have to use unaligned pointer friendly  */ \
      /* way of accessing them in order to avoid undefined behavior in C++ */ \
      /* code. */                                                             \
      return base::ReadUnalignedValue<type>(FIELD_ADDR(*this, offset));       \
    } else {                                                                  \
      return *reinterpret_cast<type const*>(FIELD_ADDR(*this, offset));       \
    }                                                                         \
  }                                                                           \
  void holder::set_##name(type value) {                                       \
    if (COMPRESS_POINTERS_BOOL && alignof(type) > kTaggedSize) {              \
      /* TODO(ishell, v8:8875): When pointer compression is enabled 8-byte */ \
      /* size fields (external pointers, doubles and BigInt data) are only */ \
      /* kTaggedSize aligned so we have to use unaligned pointer friendly  */ \
      /* way of accessing them in order to avoid undefined behavior in C++ */ \
      /* code. */                                                             \
      base::WriteUnalignedValue<type>(FIELD_ADDR(*this, offset), value);      \
    } else {                                                                  \
      *reinterpret_cast<type*>(FIELD_ADDR(*this, offset)) = value;            \
    }                                                                         \
  }

// WasmModuleObject
wasm::NativeModule* WasmModuleObject::native_module() const {
  return managed_native_module().raw();
}
const std::shared_ptr<wasm::NativeModule>&
WasmModuleObject::shared_native_module() const {
  return managed_native_module().get();
}
const wasm::WasmModule* WasmModuleObject::module() const {
  // TODO(clemensb): Remove this helper (inline in callers).
  return native_module()->module();
}
bool WasmModuleObject::is_asm_js() {
  bool asm_js = is_asmjs_module(module());
  DCHECK_EQ(asm_js, script().IsUserJavaScript());
  return asm_js;
}

// WasmMemoryObject
OPTIONAL_ACCESSORS(WasmMemoryObject, instances, WeakArrayList, kInstancesOffset)

// WasmGlobalObject
ACCESSORS(WasmGlobalObject, untagged_buffer, JSArrayBuffer,
          kUntaggedBufferOffset)
ACCESSORS(WasmGlobalObject, tagged_buffer, FixedArray, kTaggedBufferOffset)

wasm::ValueType WasmGlobalObject::type() const {
  return wasm::ValueType::FromRawBitField(static_cast<uint32_t>(raw_type()));
}
void WasmGlobalObject::set_type(wasm::ValueType value) {
  set_raw_type(static_cast<int>(value.raw_bit_field()));
}

int WasmGlobalObject::type_size() const { return type().element_size_bytes(); }

Address WasmGlobalObject::address() const {
  DCHECK_NE(type(), wasm::kWasmExternRef);
  DCHECK_LE(offset() + type_size(), untagged_buffer().byte_length());
  return Address(untagged_buffer().backing_store()) + offset();
}

int32_t WasmGlobalObject::GetI32() {
  return base::ReadUnalignedValue<int32_t>(address());
}

int64_t WasmGlobalObject::GetI64() {
  return base::ReadUnalignedValue<int64_t>(address());
}

float WasmGlobalObject::GetF32() {
  return base::ReadUnalignedValue<float>(address());
}

double WasmGlobalObject::GetF64() {
  return base::ReadUnalignedValue<double>(address());
}

Handle<Object> WasmGlobalObject::GetRef() {
  // We use this getter for externref and funcref.
  DCHECK(type().is_reference());
  return handle(tagged_buffer().get(offset()), GetIsolate());
}

void WasmGlobalObject::SetI32(int32_t value) {
  base::WriteUnalignedValue(address(), value);
}

void WasmGlobalObject::SetI64(int64_t value) {
  base::WriteUnalignedValue(address(), value);
}

void WasmGlobalObject::SetF32(float value) {
  base::WriteUnalignedValue(address(), value);
}

void WasmGlobalObject::SetF64(double value) {
  base::WriteUnalignedValue(address(), value);
}

void WasmGlobalObject::SetExternRef(Handle<Object> value) {
  DCHECK(type().is_reference_to(wasm::HeapType::kExtern) ||
         type().is_reference_to(wasm::HeapType::kAny));
  tagged_buffer().set(offset(), *value);
}

bool WasmGlobalObject::SetFuncRef(Isolate* isolate, Handle<Object> value) {
  DCHECK_EQ(type(), wasm::kWasmFuncRef);
  if (!value->IsNull(isolate) &&
      !WasmExternalFunction::IsWasmExternalFunction(*value) &&
      !WasmCapiFunction::IsWasmCapiFunction(*value)) {
    return false;
  }
  tagged_buffer().set(offset(), *value);
  return true;
}

// WasmInstanceObject
PRIMITIVE_ACCESSORS(WasmInstanceObject, memory_start, byte*, kMemoryStartOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, memory_size, size_t, kMemorySizeOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, isolate_root, Address,
                    kIsolateRootOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, stack_limit_address, Address,
                    kStackLimitAddressOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, real_stack_limit_address, Address,
                    kRealStackLimitAddressOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, new_allocation_limit_address, Address*,
                    kNewAllocationLimitAddressOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, new_allocation_top_address, Address*,
                    kNewAllocationTopAddressOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, old_allocation_limit_address, Address*,
                    kOldAllocationLimitAddressOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, old_allocation_top_address, Address*,
                    kOldAllocationTopAddressOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, imported_function_targets, Address*,
                    kImportedFunctionTargetsOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, globals_start, byte*,
                    kGlobalsStartOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, imported_mutable_globals, Address*,
                    kImportedMutableGlobalsOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, indirect_function_table_size, uint32_t,
                    kIndirectFunctionTableSizeOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, indirect_function_table_sig_ids,
                    uint32_t*, kIndirectFunctionTableSigIdsOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, indirect_function_table_targets,
                    Address*, kIndirectFunctionTableTargetsOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, jump_table_start, Address,
                    kJumpTableStartOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, data_segment_starts, Address*,
                    kDataSegmentStartsOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, data_segment_sizes, uint32_t*,
                    kDataSegmentSizesOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, dropped_elem_segments, byte*,
                    kDroppedElemSegmentsOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, hook_on_function_call_address, Address,
                    kHookOnFunctionCallAddressOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, num_liftoff_function_calls_array,
                    uint32_t*, kNumLiftoffFunctionCallsArrayOffset)
PRIMITIVE_ACCESSORS(WasmInstanceObject, break_on_entry, uint8_t,
                    kBreakOnEntryOffset)

ACCESSORS(WasmInstanceObject, module_object, WasmModuleObject,
          kModuleObjectOffset)
ACCESSORS(WasmInstanceObject, exports_object, JSObject, kExportsObjectOffset)
ACCESSORS(WasmInstanceObject, native_context, Context, kNativeContextOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, memory_object, WasmMemoryObject,
                   kMemoryObjectOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, untagged_globals_buffer, JSArrayBuffer,
                   kUntaggedGlobalsBufferOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, tagged_globals_buffer, FixedArray,
                   kTaggedGlobalsBufferOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, imported_mutable_globals_buffers,
                   FixedArray, kImportedMutableGlobalsBuffersOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, tables, FixedArray, kTablesOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, indirect_function_tables, FixedArray,
                   kIndirectFunctionTablesOffset)
ACCESSORS(WasmInstanceObject, imported_function_refs, FixedArray,
          kImportedFunctionRefsOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, indirect_function_table_refs, FixedArray,
                   kIndirectFunctionTableRefsOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, managed_native_allocations, Foreign,
                   kManagedNativeAllocationsOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, tags_table, FixedArray, kTagsTableOffset)
OPTIONAL_ACCESSORS(WasmInstanceObject, wasm_external_functions, FixedArray,
                   kWasmExternalFunctionsOffset)
ACCESSORS(WasmInstanceObject, managed_object_maps, FixedArray,
          kManagedObjectMapsOffset)

void WasmInstanceObject::clear_padding() {
  if (FIELD_SIZE(kOptionalPaddingOffset) != 0) {
    DCHECK_EQ(4, FIELD_SIZE(kOptionalPaddingOffset));
    memset(reinterpret_cast<void*>(address() + kOptionalPaddingOffset), 0,
           FIELD_SIZE(kOptionalPaddingOffset));
  }
}

IndirectFunctionTableEntry::IndirectFunctionTableEntry(
    Handle<WasmInstanceObject> instance, int table_index, int entry_index)
    : instance_(table_index == 0 ? instance
                                 : Handle<WasmInstanceObject>::null()),
      table_(table_index != 0
                 ? handle(WasmIndirectFunctionTable::cast(
                              instance->indirect_function_tables().get(
                                  table_index)),
                          instance->GetIsolate())
                 : Handle<WasmIndirectFunctionTable>::null()),
      index_(entry_index) {
  DCHECK_GE(entry_index, 0);
  DCHECK_LT(entry_index, table_index == 0
                             ? instance->indirect_function_table_size()
                             : table_->size());
}

IndirectFunctionTableEntry::IndirectFunctionTableEntry(
    Handle<WasmIndirectFunctionTable> table, int entry_index)
    : instance_(Handle<WasmInstanceObject>::null()),
      table_(table),
      index_(entry_index) {
  DCHECK_GE(entry_index, 0);
  DCHECK_LT(entry_index, table_->size());
}

ImportedFunctionEntry::ImportedFunctionEntry(
    Handle<WasmInstanceObject> instance, int index)
    : instance_(instance), index_(index) {
  DCHECK_GE(index, 0);
  DCHECK_LT(index, instance->module()->num_imported_functions);
}

// WasmExceptionPackage
OBJECT_CONSTRUCTORS_IMPL(WasmExceptionPackage, JSObject)
CAST_ACCESSOR(WasmExceptionPackage)

// WasmExportedFunction
WasmExportedFunction::WasmExportedFunction(Address ptr) : JSFunction(ptr) {
  SLOW_DCHECK(IsWasmExportedFunction(*this));
}
CAST_ACCESSOR(WasmExportedFunction)

// WasmFunctionData
ACCESSORS(WasmFunctionData, ref, Object, kRefOffset)

DEF_GETTER(WasmFunctionData, wrapper_code, Code) {
  return FromCodeT(TorqueGeneratedClass::wrapper_code(cage_base));
}
void WasmFunctionData::set_wrapper_code(Code code, WriteBarrierMode mode) {
  TorqueGeneratedClass::set_wrapper_code(ToCodeT(code), mode);
}

wasm::FunctionSig* WasmExportedFunctionData::sig() const {
  return reinterpret_cast<wasm::FunctionSig*>(signature().foreign_address());
}

// WasmJSFunction
WasmJSFunction::WasmJSFunction(Address ptr) : JSFunction(ptr) {
  SLOW_DCHECK(IsWasmJSFunction(*this));
}
CAST_ACCESSOR(WasmJSFunction)

// WasmJSFunctionData
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmJSFunctionData)
ACCESSORS(WasmJSFunctionData, raw_wasm_to_js_wrapper_code, CodeT,
          kWasmToJsWrapperCodeOffset)

DEF_GETTER(WasmJSFunctionData, wasm_to_js_wrapper_code, Code) {
  return FromCodeT(raw_wasm_to_js_wrapper_code(cage_base));
}
void WasmJSFunctionData::set_wasm_to_js_wrapper_code(Code code,
                                                     WriteBarrierMode mode) {
  set_raw_wasm_to_js_wrapper_code(ToCodeT(code), mode);
}

// WasmCapiFunction
WasmCapiFunction::WasmCapiFunction(Address ptr) : JSFunction(ptr) {
  SLOW_DCHECK(IsWasmCapiFunction(*this));
}
CAST_ACCESSOR(WasmCapiFunction)

// WasmExternalFunction
WasmExternalFunction::WasmExternalFunction(Address ptr) : JSFunction(ptr) {
  SLOW_DCHECK(IsWasmExternalFunction(*this));
}
CAST_ACCESSOR(WasmExternalFunction)

// WasmIndirectFunctionTable
TQ_OBJECT_CONSTRUCTORS_IMPL(WasmIndirectFunctionTable)
PRIMITIVE_ACCESSORS(WasmIndirectFunctionTable, sig_ids, uint32_t*,
                    kSigIdsOffset)
PRIMITIVE_ACCESSORS(WasmIndirectFunctionTable, targets, Address*,
                    kTargetsOffset)
OPTIONAL_ACCESSORS(WasmIndirectFunctionTable, managed_native_allocations,
                   Foreign, kManagedNativeAllocationsOffset)

#undef OPTIONAL_ACCESSORS
#undef READ_PRIMITIVE_FIELD
#undef WRITE_PRIMITIVE_FIELD
#undef PRIMITIVE_ACCESSORS

wasm::ValueType WasmTableObject::type() {
  return wasm::ValueType::FromRawBitField(raw_type());
}

bool WasmMemoryObject::has_maximum_pages() { return maximum_pages() >= 0; }

// static
Handle<Object> WasmObject::ReadValueAt(Isolate* isolate, Handle<HeapObject> obj,
                                       wasm::ValueType type, uint32_t offset) {
  Address field_address = obj->GetFieldAddress(offset);
  switch (type.kind()) {
    case wasm::kI8: {
      int8_t value = base::Memory<int8_t>(field_address);
      return handle(Smi::FromInt(value), isolate);
    }
    case wasm::kI16: {
      int16_t value = base::Memory<int16_t>(field_address);
      return handle(Smi::FromInt(value), isolate);
    }
    case wasm::kI32: {
      int32_t value = base::Memory<int32_t>(field_address);
      return isolate->factory()->NewNumberFromInt(value);
    }
    case wasm::kI64: {
      int64_t value = base::ReadUnalignedValue<int64_t>(field_address);
      return BigInt::FromInt64(isolate, value);
    }
    case wasm::kF32: {
      float value = base::Memory<float>(field_address);
      return isolate->factory()->NewNumber(value);
    }
    case wasm::kF64: {
      double value = base::ReadUnalignedValue<double>(field_address);
      return isolate->factory()->NewNumber(value);
    }
    case wasm::kS128:
      // TODO(v8:11804): implement
      UNREACHABLE();

    case wasm::kRef:
    case wasm::kOptRef: {
      ObjectSlot slot(field_address);
      return handle(slot.load(isolate), isolate);
    }

    case wasm::kRtt:
    case wasm::kRttWithDepth:
      // Rtt values are not supposed to be made available to JavaScript side.
      UNREACHABLE();

    case wasm::kVoid:
    case wasm::kBottom:
      UNREACHABLE();
  }
}

// static
MaybeHandle<Object> WasmObject::ToWasmValue(Isolate* isolate,
                                            wasm::ValueType type,
                                            Handle<Object> value) {
  switch (type.kind()) {
    case wasm::kI8:
    case wasm::kI16:
    case wasm::kI32:
    case wasm::kF32:
    case wasm::kF64:
      return Object::ToNumber(isolate, value);

    case wasm::kI64:
      return BigInt::FromObject(isolate, value);

    case wasm::kRef:
    case wasm::kOptRef: {
      // TODO(v8:11804): implement ref type check
      UNREACHABLE();
    }

    case wasm::kS128:
      // TODO(v8:11804): implement
      UNREACHABLE();

    case wasm::kRtt:
    case wasm::kRttWithDepth:
      // Rtt values are not supposed to be made available to JavaScript side.
      UNREACHABLE();

    case wasm::kVoid:
    case wasm::kBottom:
      UNREACHABLE();
  }
}

// Conversions from Numeric objects.
// static
template <typename ElementType>
ElementType WasmObject::FromNumber(Object value) {
  // The value must already be prepared for storing to numeric fields.
  DCHECK(value.IsNumber());
  if (value.IsSmi()) {
    return static_cast<ElementType>(Smi::ToInt(value));

  } else if (value.IsHeapNumber()) {
    double double_value = HeapNumber::cast(value).value();
    if (std::is_same<ElementType, double>::value ||
        std::is_same<ElementType, float>::value) {
      return static_cast<ElementType>(double_value);
    } else {
      CHECK(std::is_integral<ElementType>::value);
      return static_cast<ElementType>(DoubleToInt32(double_value));
    }
  }
  UNREACHABLE();
}

// static
void WasmObject::WriteValueAt(Isolate* isolate, Handle<HeapObject> obj,
                              wasm::ValueType type, uint32_t offset,
                              Handle<Object> value) {
  Address field_address = obj->GetFieldAddress(offset);
  switch (type.kind()) {
    case wasm::kI8: {
      auto scalar_value = FromNumber<int8_t>(*value);
      base::Memory<int8_t>(field_address) = scalar_value;
      break;
    }
    case wasm::kI16: {
      auto scalar_value = FromNumber<int16_t>(*value);
      base::Memory<int16_t>(field_address) = scalar_value;
      break;
    }
    case wasm::kI32: {
      auto scalar_value = FromNumber<int32_t>(*value);
      base::Memory<int32_t>(field_address) = scalar_value;
      break;
    }
    case wasm::kI64: {
      int64_t scalar_value = BigInt::cast(*value).AsInt64();
      base::WriteUnalignedValue<int64_t>(field_address, scalar_value);
      break;
    }
    case wasm::kF32: {
      auto scalar_value = FromNumber<float>(*value);
      base::Memory<float>(field_address) = scalar_value;
      break;
    }
    case wasm::kF64: {
      auto scalar_value = FromNumber<double>(*value);
      base::WriteUnalignedValue<double>(field_address, scalar_value);
      break;
    }
    case wasm::kRef:
    case wasm::kOptRef:
      // TODO(v8:11804): implement
      UNREACHABLE();

    case wasm::kS128:
      // TODO(v8:11804): implement
      UNREACHABLE();

    case wasm::kRtt:
    case wasm::kRttWithDepth:
      // Rtt values are not supposed to be made available to JavaScript side.
      UNREACHABLE();

    case wasm::kVoid:
    case wasm::kBottom:
      UNREACHABLE();
  }
}

wasm::StructType* WasmStruct::type(Map map) {
  WasmTypeInfo type_info = map.wasm_type_info();
  return reinterpret_cast<wasm::StructType*>(type_info.foreign_address());
}

wasm::StructType* WasmStruct::GcSafeType(Map map) {
  DCHECK_EQ(WASM_STRUCT_TYPE, map.instance_type());
  HeapObject raw = HeapObject::cast(map.constructor_or_back_pointer());
  MapWord map_word = raw.map_word(kRelaxedLoad);
  HeapObject forwarded =
      map_word.IsForwardingAddress() ? map_word.ToForwardingAddress() : raw;
  Foreign foreign = Foreign::cast(forwarded);
  return reinterpret_cast<wasm::StructType*>(foreign.foreign_address());
}

int WasmStruct::Size(const wasm::StructType* type) {
  // Object size must fit into a Smi (because of filler objects), and its
  // computation must not overflow.
  STATIC_ASSERT(Smi::kMaxValue <= kMaxInt);
  DCHECK_LE(type->total_fields_size(), Smi::kMaxValue - kHeaderSize);
  return std::max(kHeaderSize + static_cast<int>(type->total_fields_size()),
                  Heap::kMinObjectSizeInTaggedWords * kTaggedSize);
}

int WasmStruct::GcSafeSize(Map map) {
  wasm::StructType* type = GcSafeType(map);
  return Size(type);
}

wasm::StructType* WasmStruct::type() const { return type(map()); }

Address WasmStruct::RawFieldAddress(int raw_offset) {
  int offset = WasmStruct::kHeaderSize + raw_offset;
  return FIELD_ADDR(*this, offset);
}

ObjectSlot WasmStruct::RawField(int raw_offset) {
  return ObjectSlot(RawFieldAddress(raw_offset));
}

// static
Handle<Object> WasmStruct::GetField(Isolate* isolate, Handle<WasmStruct> obj,
                                    uint32_t field_index) {
  wasm::StructType* type = obj->type();
  CHECK_LT(field_index, type->field_count());
  wasm::ValueType field_type = type->field(field_index);
  int offset = WasmStruct::kHeaderSize + type->field_offset(field_index);
  return ReadValueAt(isolate, obj, field_type, offset);
}

// static
void WasmStruct::SetField(Isolate* isolate, Handle<WasmStruct> obj,
                          uint32_t field_index, Handle<Object> value) {
  wasm::StructType* type = obj->type();
  CHECK_LT(field_index, type->field_count());
  wasm::ValueType field_type = type->field(field_index);
  int offset = WasmStruct::kHeaderSize + type->field_offset(field_index);
  WriteValueAt(isolate, obj, field_type, offset, value);
}

wasm::ArrayType* WasmArray::type(Map map) {
  DCHECK_EQ(WASM_ARRAY_TYPE, map.instance_type());
  WasmTypeInfo type_info = map.wasm_type_info();
  return reinterpret_cast<wasm::ArrayType*>(type_info.foreign_address());
}

wasm::ArrayType* WasmArray::GcSafeType(Map map) {
  DCHECK_EQ(WASM_ARRAY_TYPE, map.instance_type());
  HeapObject raw = HeapObject::cast(map.constructor_or_back_pointer());
  MapWord map_word = raw.map_word(kRelaxedLoad);
  HeapObject forwarded =
      map_word.IsForwardingAddress() ? map_word.ToForwardingAddress() : raw;
  Foreign foreign = Foreign::cast(forwarded);
  return reinterpret_cast<wasm::ArrayType*>(foreign.foreign_address());
}

wasm::ArrayType* WasmArray::type() const { return type(map()); }

int WasmArray::SizeFor(Map map, int length) {
  int element_size = type(map)->element_type().element_size_bytes();
  return kHeaderSize + RoundUp(element_size * length, kTaggedSize);
}

int WasmArray::GcSafeSizeFor(Map map, int length) {
  int element_size = GcSafeType(map)->element_type().element_size_bytes();
  return kHeaderSize + RoundUp(element_size * length, kTaggedSize);
}

// static
Handle<Object> WasmArray::GetElement(Isolate* isolate, Handle<WasmArray> array,
                                     uint32_t index) {
  if (index >= array->length()) {
    return isolate->factory()->undefined_value();
  }
  wasm::ValueType element_type = array->type()->element_type();
  uint32_t offset =
      WasmArray::kHeaderSize + index * element_type.element_size_bytes();
  return ReadValueAt(isolate, array, element_type, offset);
}

void WasmTypeInfo::clear_foreign_address(Isolate* isolate) {
#ifdef V8_HEAP_SANDBOX
  // Due to the type-specific pointer tags for external pointers, we need to
  // allocate an entry in the table here even though it will just store nullptr.
  AllocateExternalPointerEntries(isolate);
#endif
  set_foreign_address(isolate, 0);
}

#include "src/objects/object-macros-undef.h"

}  // namespace internal
}  // namespace v8

#endif  // V8_WASM_WASM_OBJECTS_INL_H_