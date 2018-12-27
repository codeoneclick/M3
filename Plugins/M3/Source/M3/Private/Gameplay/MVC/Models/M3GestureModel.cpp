// Copyright serhii serhiiv 2018 All rights reserved.

#include "M3GestureModel.h"

M3GestureModel::M3GestureModel() {
}

M3GestureModel::~M3GestureModel() {
}

void M3GestureModel::Serialize() {

}

void M3GestureModel::Deserialize(AM3Scheme_INTERFACE* Scheme) {

}

void M3GestureModel::Init() {

}

int M3GestureModel::GetGesturesNum() const {
	return Entity->Get()->Gestures->Get()->size();
}

bool M3GestureModel::IsLastGestureIs(EM3Gesture _Gesture) const {
	bool Result = false;
	if (Entity->Get()->Gestures->Get()->size() != 0) {
		Result = Entity->Get()->Gestures->Get()->back().Gesture == _Gesture;
	}
	return Result;
}

void M3GestureModel::PushGesture(EM3Gesture _Gesture, const FVector& _Location) {
	M3Gesture Gesture;
	Gesture.Gesture = _Gesture;
	Gesture.Location = _Location;
	Entity->Get()->Gestures->Get()->push_back(Gesture);
}

M3Gesture M3GestureModel::PopGesture() {
	M3Gesture Gesture = Entity->Get()->Gestures->Get()->front();
	Entity->Get()->Gestures->Get()->pop_front();
	return Gesture;
}
