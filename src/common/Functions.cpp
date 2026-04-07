#include <sstream>
#include <iomanip>

namespace project::common
{

    std::string to_string_precision(double value, int precision = 2)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        return oss.str();
    }
}