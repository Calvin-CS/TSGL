  /*! \brief Enum for valid states for the Dining Philosophers
  */
  enum PhilState {
    hasNone, hasRight, hasLeft, hasBoth, isFull, thinking
  };

  /*! \brief Enum for valid actions for the Dining Philosophers
  */
  enum PhilAction {
    doNothing, tryLeft, tryRight, tryBoth, releaseLeft, releaseRight, releaseBoth
  };

  /*! \brief Enum for resource collision resolution methods for the Dining Philosophers' problem
  */
  enum PhilMethod {
    forfeitWhenBlocked, waitWhenBlocked, nFrameRelease, resourceHierarchy, oddEven
  };
