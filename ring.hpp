#pragma once
#include<cassert>

class ZmodP {
public:
	ZmodP(int initial, int P) :data(initial), P(P) {
		cancel();
	}

	int P;
	int data;

	//
	//implementations of ==, != operators modular P
	//
	bool operator==(ZmodP &other) {
		assert(this->P == other.P);
		this->cancel();
		other.cancel();
		return this->data == other.data;
	}
	bool operator!=(ZmodP &other) {
		return !(*this == other);
	}


	//
	//	implementations of basic arithmetic (+, -, /, *) operators with modular P
	//
	ZmodP &operator+=(ZmodP &other) {
		assert(this->P == other.P);
		this->data += other.data;
		this->cancel();
		return *this;
	}
	ZmodP operator+(ZmodP &other) {
		assert(this->P == other.P);
		auto result = *this;
		result += other;
		return result;
	}

	ZmodP &operator-=(ZmodP &other) {
		assert(this->P == other.P);
		this->data -= other.data;
		this->cancel();
		return *this;
	}
	ZmodP operator-(ZmodP &other) {
		assert(this->P == other.P);
		auto result = *this;
		result -= other;
		return result;
	}

	ZmodP &operator*=(ZmodP &other) {
		assert(this->P == other.P);
		this->data *= other.data;
		this->cancel();
		return *this;
	}
	ZmodP operator*(ZmodP &other) {
		assert(this->P == other.P);
		auto result = *this;
		result *= other;
		return result;
	}

	ZmodP &operator/=(ZmodP &other) {
		assert(this->P == other.P);
		assert(other.data != 0);
		return *this *= other.inverse();
	}
	ZmodP operator/(ZmodP &other) {
		assert(this->P == other.P);
		assert(other.data != 0);
		auto result = *this;
		result /= other;
		return result;
	}




private:
	void cancel() {
		if (this->need_cancel()) {
			data %= P;
			if (data < 0) {
				data += P;
			}
		}
	}

	bool need_cancel() {
		if (this->data > this->P) {
			return true;
		}
		else if (this->data < 0) {
			return true;
		}
		else {
			return false;
		}
	}

public:

	ZmodP inverse() {
		auto p = this->P;
		auto num_old = this->data;
		auto num_new = num_old;
		auto mult = this->data;

		while (num_new != 1) {
			num_old = num_new;
			num_new *= mult;
			num_new %= p;
		}


		return ZmodP(num_old, p);
	}

};