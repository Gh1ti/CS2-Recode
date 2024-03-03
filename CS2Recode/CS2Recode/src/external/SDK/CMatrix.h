#pragma once
struct CMatrix4x4 {
	float data[4][4];

	float* operator[](int i){
		return data[i];
	}
	const float* operator[](int i) const noexcept {
		return data[i];
	}
};