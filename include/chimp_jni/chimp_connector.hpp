#ifndef CHIMP_CONNECTOR_HPP
#define CHIMP_CONNECTOR_HPP

#include "plan.hpp"

namespace chimp_jni
{

class ChimpConnector
{

  public:
    ChimpConnector();

    ~ChimpConnector();

    Plan callChimp();
};

} // namespace chimp_jni

#endif // CHIMP_CONNECTOR_HPP