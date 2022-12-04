package com.diningdiego;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

@Entity
public class Hour {
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private int id;
	
	@Column
	private int hour;
	
	@Column(name="people_entered")
	private int peopleEntered;

	public int getHour() {
		return hour;
	}

	public void setHour(int hour) {
		this.hour = hour;
	}

	public int getPeopleEntered() {
		return peopleEntered;
	}

	public void setPeopleEntered(int peopleEntered) {
		this.peopleEntered = peopleEntered;
	}
}
