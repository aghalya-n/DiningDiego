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
@RequestMapping("/dininghall")
@CrossOrigin
public class DiningHallController {
	@Autowired
	DiningHallService service;
	
	@PostMapping()
	public DiningHall createDay(@RequestBody DiningHall diningHall) {
		return service.createDiningHall(diningHall);
	}
	@GetMapping()
	public List<DiningHall> getDays() {
		return service.getAllDiningHalls();
	}
}
