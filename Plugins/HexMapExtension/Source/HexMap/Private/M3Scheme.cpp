// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Scheme.h"

AM3Scheme_INTERFACE::AM3Scheme_INTERFACE() {
}

AM3Scheme_INTERFACE::~AM3Scheme_INTERFACE() {
}

AM3CellAppointmentScheme::AM3CellAppointmentScheme() {
}

AM3CellAppointmentScheme::~AM3CellAppointmentScheme() {

}

AM3CellScheme::AM3CellScheme() {

}

AM3CellScheme::~AM3CellScheme() {

}

void AM3CellScheme::AddAppointment(AM3CellAppointmentScheme* Appointment) {
	Appointments.Add(Appointment);
}

AM3CellAppointmentScheme* AM3CellScheme::GetAppointment(EM3CellAppointment AppointmentId) const {
	AM3CellAppointmentScheme* Appointment = nullptr;

	for (const auto& It : Appointments) {
		if (It->Appointment == AppointmentId) {
			Appointment = It;
			break;
		}
	}

	return Appointment;
}

AM3BoardScheme::AM3BoardScheme() {

}

AM3BoardScheme::~AM3BoardScheme() {

}