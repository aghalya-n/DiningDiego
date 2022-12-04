package com.diningdiego;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/day")
@CrossOrigin
public class DayController {
	@Autowired
	DayService service;
	
	@PostMapping()
	public Day createDay(@RequestBody Day day) {
		return service.createDay(day);
	}
	@GetMapping()
	public List<Day> getDays() {
		return service.getDays();
	}
}