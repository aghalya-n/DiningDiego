package com.diningdiego;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class DayService {
	@Autowired
	DayRepository repo;
	
	public Day createDay(Day day) {
		return repo.save(day);
	}

	public List<Day> getDays() {
		return repo.findAll();
	}
}
