

#include "../Check.hpp"

#include "../../Types/Option.hpp"
#include "gtest/gtest.h"


TEST(check, option_check) {

    let opt = Option<std::string>::some("salam");

    check_option_ret_void_(opt, "Opt has no value!");
}
