class Function {
 public:
  Function() {}

  virtual ~Function() {}

  virtual Decimal valueAt(Decimal x) const = 0;
};

class RoundFunction : public Function {
 public:
  virtual Decimal valueAt(Decimal x) const {
    return round(x);
  }
};
