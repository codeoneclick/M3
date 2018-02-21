// Fill out your copyright notice in the Description page of Project Settings.

#include "M3Asset.h"

FM3Asset::FM3Asset() : Asset(EM3Asset::UNKNOWN),
Filepath("") {
}

FM3Asset::~FM3Asset() {
}

FM3BoardAsset::FM3BoardAsset() {
	Asset = EM3Asset::COMPOSITE;
}

FM3BoardAsset::~FM3BoardAsset() {

}
