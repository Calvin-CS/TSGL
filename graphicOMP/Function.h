/*
 * Function.h provides functions for drawing with CartesianCanvas
 *
 * Created on: Jun 11, 2014
 * Author: Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/12/2014
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <cmath>

class Function {
public:
	Function() {}
	virtual ~Function() {}
	virtual long double valueAt(long double x) const = 0;
};

class PowerFunction : public Function {
public:
	PowerFunction(long double a) {
		a_ = a;
	}

	virtual long double valueAt(long double x) const {
		return pow(x, a_);
	}
private:
	long double a_;
};

class SquareRootFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return sqrt(x);
	}
};

class CubedRootFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return cbrt(x);
	}
};

class SineFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return sin(x);
	}
};

class CosineFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return cos(x);
	}
};

class TangentFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return tan(x);
	}
};

class AbsoluteFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return abs(x);
	}
};

class ExponentialFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return exp(x);
	}
};

class NaturalLogFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return log(x);
	}
};

class CommonLogFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return log10(x);
	}
};

class CeilingFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return ceil(x);
	}
};

class FloorFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return floor(x);
	}
};

class RoundFunction : public Function {
public:
	virtual long double valueAt(long double x) const {
		return round(x);
	}
};

#endif /* FUNCTION_H_ */
