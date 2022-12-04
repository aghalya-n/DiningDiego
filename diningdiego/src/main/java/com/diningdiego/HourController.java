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
@RequestMapping("/hour")
@CrossOrigin
public class HourController {
	@Autowired
	HourService service;
	
	@PostMapping()
	public Hour createDay(@RequestBody Hour hour) {
		return service.createHour(hour);
	}
	@GetMapping()
	public List<Hour> getDays() {
		return service.getHours();
	}
	
}
