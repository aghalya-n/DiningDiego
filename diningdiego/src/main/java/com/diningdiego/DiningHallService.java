package com.diningdiego;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class DiningHallService {
	@Autowired
	DiningHallRepository repo;
	
	public DiningHall createDiningHall(DiningHall diningHall) {
		return repo.save(diningHall);
	}
	public List<DiningHall> getAllDiningHalls() {
		return repo.findAll();
	}
}
