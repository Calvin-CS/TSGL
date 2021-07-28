/*
 * Function.h provides functions for drawing with CartesianCanvas.
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "Util.h"

namespace tsgl {

/*! \class Function
 *  \brief A base class for creating mathematical functions plottable by a CartesianCanvas.
 *  \details Function provides a base class for the creation of mathematical functions.
 *   By extending this class and overriding the valueAt() method, users can easily plot the values
 *   of their function on a CartesianCanvas.
 *  \details A number of pre-built Function subclasses are included in the header file for reference.
 */
class Function {
 public:

    /*!
     * \brief Constructs a new Function.
     * \details This is the default constructor for the Function class.
     * \note The default constructor for the parent Function class does absolutely nothing. Any construction should be defined in the subclass.
     */
    Function() {}

    /*!
     * \brief Destructor for the Function class.
     * \note The default destructor for the parent Function class does absolutely nothing. Any destruction should be defined in the subclass.
     */
    virtual ~Function() {}

    /*!
     * \brief Method to determine the value of a Function subclass.
     * \details This method should be overridden with the actual function you want to compute.
     *      \param x The input to the function.  Assuming your function is F, x will be used to compute F(x).
     * \return The Decimal value of F(x).
     * \note This method is abstract and <B>must</B> be overridden.
     */
    virtual Decimal valueAt(Decimal x) const = 0;
};

/*! \class PowerFunction
 *  \brief Function to compute the input raised to a specified power.
 */
class PowerFunction : public Function {
 public:

    /*!
     * \brief Constructs a new PowerFunction.
     *      \param a The power to which the input is raised.
     */
    PowerFunction(Decimal a) {
        a_ = a;
    }

    /*!
     * \brief Method to determine the value of PowerFunction.
     *      \param x The input to the function.
     * \return *x* raised to the *a*th power.
     */
    virtual Decimal valueAt(Decimal x) const {
        return pow(x, a_);
    }
 private:
    Decimal a_;
};

/*! \class SquareRootFunction
 *  \brief Function to compute the square root of the input.
 */
class SquareRootFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of SquareRootFunction.
     * \return The square root of *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return sqrt(x);
    }
};

/*! \class SineFunction
 *  \brief Function to compute the sine of the input.
 */
class SineFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of SineFunction.
     * \return The sine of *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return sin(x);
    }
};

/*! \class CosineFunction
 *  \brief Function to compute the cosine of the input.
 */
class CosineFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of CosineFunction.
     * \return The cosine of *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return cos(x);
    }
};

/*! \class TangentFunction
 *  \brief Function to compute the tangent of the input.
 */
class TangentFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of TangentFunction.
     * \return The tangent of *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return tan(x);
    }
};

/*! \class AbsoluteFunction
 *  \brief Function to compute the absolute value of the input.
 */
class AbsoluteFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of AbsoluteFunction.
     * \return The absolute value of *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return std::abs(x);
    }
};

/*! \class ExponentialFunction
 *  \brief Function to compute e raised to the input.
 */
class ExponentialFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of ExponentialFunction.
     * \return *e* raised to the power of *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return exp(x);
    }
};

/*! \class NaturalLogFunction
 *  \brief Function to compute the natural log of the input.
 */
class NaturalLogFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of NaturalLogFunction.
     * \return The natural log of *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return log(x);
    }
};

/*! \class CommonLogFunction
 *  \brief Function to compute the base 10 log of the input.
 */
class CommonLogFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of CommonLogFunction.
     * \return The base 10 log of *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return log10(x);
    }
};

/*! \class CeilingFunction
 *  \brief Function to compute the mathematical ceiling of the input.
 */
class CeilingFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of CeilingFunction.
     * \return The smallest integer greater than or equal to *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return ceil(x);
    }
};

/*! \class FloorFunction
 *  \brief Function to compute the mathematical floor of the input.
 */
class FloorFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of FloorFunction.
     * \return The largest integer less than or equal to *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return floor(x);
    }
};

/*! \class RoundFunction
 *  \brief Function to round the input to the nearest integer number.
 */
class RoundFunction : public Function {
 public:

    /*!
     * \brief Method to determine the value of RoundFunction.
     * \return The closest integer to *x*.
     */
    virtual Decimal valueAt(Decimal x) const {
        return round(x);
    }
};

}

#endif /* FUNCTION_H_ */
