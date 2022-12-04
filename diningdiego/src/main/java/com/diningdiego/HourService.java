package com.diningdiego;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
@Service
public class HourService {
	@Autowired
	HourRepository repo;
	
	public Hour createHour(Hour hour) {
		return repo.save(hour);
	}
	public List<Hour> getHours() {
		return repo.findAll();
	}
	
}
