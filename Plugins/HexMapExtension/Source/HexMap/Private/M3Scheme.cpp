// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Scheme.h"

void UM3CellScheme::AddAppointment(UM3CellAppointmentScheme* Appointment) {
	Appointments.Add(Appointment);
}

UM3CellAppointmentScheme* UM3CellScheme::GetAppointment(EM3CellAppointment AppointmentId) const {
	UM3CellAppointmentScheme* Appointment = nullptr;

	for (const auto& It : Appointments) {
		if (It->Appointment == AppointmentId) {
			Appointment = It;
			break;
		}
	}

	return Appointment;
}