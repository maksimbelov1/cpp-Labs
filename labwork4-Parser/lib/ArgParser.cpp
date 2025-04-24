#include "ArgParser.h"
using namespace ArgumentParser;


ArgParser::ArgParser(const std::string& parser_name) {
    parser_name_ = parser_name;
}

ArgParser& ArgParser::AddStringArgument(const char short_name, const std::string& long_name, const std::string& description) {
    for (Argument& arg : arguments) {
        if (arg.long_name == long_name) {
            return *this;
        }
    }
    arguments.push_back(Argument("str", short_name, long_name, description));
    help_information = help_information + "<string>    " + short_name +  "           "  + long_name + "           " + description + "\n";

    last_add_argument = long_name; 
    return *this;
}
ArgParser& ArgParser::AddStringArgument(const std::string& long_name, const std::string& description) {
    return AddStringArgument('\0', long_name, description);
}

ArgParser& ArgParser::AddIntArgument(const char short_name, const std::string& long_name, const std::string& description) {
    for (Argument& arg : arguments) {
        if (arg.long_name == long_name) {
            return *this;
        }
    }
    arguments.push_back(Argument("int", short_name, long_name, description));
    help_information = help_information + "<int>        " + short_name +  "           "  + long_name + "           " + description + "\n";
    last_add_argument = long_name; 
    return *this;
}
ArgParser& ArgParser::AddIntArgument(const std::string& long_name, const std::string& description) {
    return AddIntArgument('\0', long_name, description);
}

ArgParser& ArgParser::AddFlag(const char short_name, const std::string& long_name, const std::string& description) {
    for (Argument& arg : arguments) {
        if (arg.long_name == long_name) {
            return *this;
        }
    }
    arguments.push_back(Argument("flag", short_name, long_name, description));
    help_information = help_information + "<bool>      "  + short_name +  "           "  + long_name + "           " + description + "\n";
    last_add_argument = long_name; 
    Default(false);
    return *this;
}
ArgParser& ArgParser::AddFlag(const std::string& long_name, const std::string& description) {
    return AddFlag('\0', long_name, description);
}

void ArgParser::AddHelp(const char short_name, const std::string& long_name, const std::string& description) {
    help_short_name = short_name;
    help_long_name = long_name;
    help_information = help_information + "            " + short_name +  "           "  + long_name + "            Display this help and exit\n";
    is_help_added = true;
}
bool ArgParser::Help() {
    return is_help_added;
}
void ArgParser::HelpDescription() {
    std::cout << parser_name_ << '\n' << help_information;
}

ArgParser& ArgParser::Default(const char* value) {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.string_value.push_back(value);
            arg.is_added = true;
            return *this;
        }
    }
    return *this;
}
ArgParser& ArgParser::Default(const int32_t value) {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.int_value.push_back(value);
            arg.is_added = true;
            return *this;
        }
    }
    return *this;
}
ArgParser& ArgParser::Default(const bool value) {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            if (arg.bool_value.size() == 0) {
                arg.bool_value.push_back(value);
            } else {
                arg.bool_value[0] = value;
            }
            arg.is_added = true;
            return *this;
        }
    }
    return *this;
}

ArgParser& ArgParser::MultiValue(size_t min_number_of_arguments) {
        for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.multi_value = static_cast<int32_t>(min_number_of_arguments);
            arg.is_multi_value = true;
            return *this;
        }
    }
    return *this;
}

ArgParser& ArgParser::Positional() {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.is_positional = true;
            return *this;
        }
    }
    return *this;
} 

ArgParser& ArgParser::StoreValue(std::string& value_name) {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.string_ptr = &value_name;
            return *this;
        }
    }
    return *this;
}
ArgParser& ArgParser::StoreValue(int32_t& value_name) {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.int_ptr = &value_name;
            return *this;
        }
    }
    return *this;
}
ArgParser& ArgParser::StoreValue(bool& value_name) {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.bool_ptr = &value_name;
            return *this;
        }
    }
    return *this;
}

ArgParser& ArgParser::StoreValues(std::vector<std::string>& values_name) {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.string_vector_ptr = &values_name;
            return *this;
        }
    }
    return *this;
}
ArgParser& ArgParser::StoreValues(std::vector<int32_t>& values_name) {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.int_vector_ptr = &values_name;
            return *this;
        }
    }
    return *this;
}
ArgParser& ArgParser::StoreValues(std::vector<bool>& values_name) {
    for (Argument& arg : arguments) {
        if (arg.long_name == last_add_argument) {
            arg.bool_vector_ptr = &values_name;
            return *this;
        }
    }
    return *this;
}
std::string ArgParser::GetStringValue(const std::string& name, const int32_t index) {
    for (Argument& arg : arguments) {
        if (arg.long_name == name) {
            if(index >= arg.string_value.size()) {
                std::cerr << "GetStringValue: index " << index << " >= number of values" << '\n';
                return std::string{};
            }
            return arg.string_value[index];
        }
    }
    return std::string{};
}
int32_t ArgParser::GetIntValue(const std::string& name, const int32_t index) {
    for (Argument& arg : arguments) {
        if (arg.long_name == name) {
            if(index >= arg.int_value.size()) {
                std::cerr << "GetIntValue: index " << index << " >= number of values" << '\n';
                return 0;
            }
            return arg.int_value[index];
        }
    }
    return 0;
}
bool ArgParser::GetFlag(const std::string& name, const int32_t index) {
    for (Argument& arg : arguments) {
        if (arg.long_name == name) {
            if (index >= arg.bool_value.size()) {
                std::cerr << "GetIntValue: index " << index << " >= number of values" << '\n';
                return false;
            }
            return arg.bool_value[index];
        }
    }
    return false;
}

bool ArgParser::Parse(const std::vector<std::string>& args) {
    for (size_t i = 1; i < args.size(); ++i) {
        found = false;
        const std::string& current_arg = args[i];
        if (current_arg.size() >= 2 && current_arg[0] == '-' && current_arg[1] == '-') {
            std::string long_name = current_arg.substr(2);
            if (is_help_added && (long_name == help_long_name)) {
                HelpDescription();
                return true;
            }
            for (int j = 0; j < long_name.size(); ++j) {
                if (long_name[j] == '=') {
                    value = long_name.substr(j + 1);
                    long_name = long_name.substr(0, j);
                    for (Argument& found_arg : arguments) {
                        if (found_arg.long_name == long_name) {
                            found = true;
                            if (found_arg.is_multi_value) {
                                last_parsed_argument_long_name = long_name;
                                last_parsed_argument_short_name = '#';
                            } else {
                                last_parsed_argument_long_name = "#";
                                last_parsed_argument_short_name = '#';
                            }
                            if (!Push_value_in_arguments(found_arg, value)) {
                                return false;
                            }
                        }
                    }
                    break;
                }
            }
            for (Argument& found_arg : arguments) {
                if (found_arg.long_name == long_name) {
                    found = true;
                    if (found_arg.argument_type == "flag") {
                        found_arg.bool_value[0] = true;
                        if (found_arg.bool_ptr) {
                            *found_arg.bool_ptr = true;
                        }
                        last_parsed_argument_long_name = "#";
                        last_parsed_argument_short_name = '#';
                    } else {
                        last_parsed_argument_long_name = long_name;
                        last_parsed_argument_short_name = '#'; 
                    }
                    break;
                }
            }
        } else if (current_arg.size() >= 2 && current_arg[0] == '-' && std::isalpha(current_arg[1])) {
            char short_name = current_arg[1];
            if (is_help_added && (short_name == help_short_name)) {
                HelpDescription();
                return true;
            }
            for (Argument& found_arg : arguments) {
                if (found_arg.short_name == short_name) {
                    found = true;
                    if (found_arg.argument_type == "flag") {
                        for (int j = 1; j < current_arg.size(); ++j) {
                            short_name = current_arg[j];
                            for (Argument& found_arg : arguments) {
                                if (found_arg.short_name == short_name) {
                                    found_arg.bool_value[0] = true;
                                    if (found_arg.bool_ptr) {
                                        *found_arg.bool_ptr = true;
                                    }
                                }
                            }
                        }
                    } else if (current_arg.size() >= 3 && current_arg[2] == '=') {
                        if (found_arg.is_multi_value) {
                            last_parsed_argument_long_name = "#";
                            last_parsed_argument_short_name = short_name;
                        } else {
                            last_parsed_argument_long_name = "#";
                            last_parsed_argument_short_name = '#';
                        }
                        value = current_arg.substr(3);
                        if (!Push_value_in_arguments(found_arg, value)) {
                            return false;
                        }
                    } else {
                        last_parsed_argument_long_name = "#";
                        last_parsed_argument_short_name = short_name; 
                    }
                    break;
                }
            }
        } else if (last_parsed_argument_long_name != "#" || last_parsed_argument_short_name != '#') {
            value = current_arg;
            for (Argument& found_arg : arguments) {
                if (found_arg.long_name == last_parsed_argument_long_name || found_arg.short_name == last_parsed_argument_short_name) {
                    found = true;
                    if (!Push_value_in_arguments(found_arg, value)) {
                        return false;
                    }
                    if (!found_arg.is_multi_value) {
                        last_parsed_argument_long_name = "#";
                        last_parsed_argument_short_name = '#';
                    }    
                }
            }
        } else {
            value = current_arg;
            for (Argument& found_arg : arguments) {
                if (found_arg.is_positional) {
                    if (found_arg.is_multi_value || (!found_arg.is_multi_value && !found_arg.is_added)) {
                        found = true;
                        if (!Push_value_in_arguments(found_arg, value)) {
                            return false;
                        }
                        break;
                    }
                }
            }
        }
        if (!found) {
            return false;
        }
    }
    return is_parsing_successful();
}
bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);
    return Parse(args);
}

bool ArgParser::is_parsing_successful() {
    for (Argument& arg : arguments) {
        if (!arg.is_added) {
            std::cerr << arg.long_name << " not found" << '\n';
            return false;
        }
        if (arg.is_multi_value && !(arg.multi_value <= arg.bool_value.size() || arg.multi_value <= arg.int_value.size() || arg.multi_value <= arg.string_value.size())) {
            std::cerr << "number of values " << arg.long_name << " is less MinArgsCount" << '\n';
            return false;
        }
    }
    return true;
}

bool ArgParser::GetInt(std::string& value, std::string& name) {
    try {
        int_value = std::stoi(value);
    } catch (const std::invalid_argument&) {
        std::cerr << "Parse:: invalid int value: " << name << '\n';
        return false;
    }
    return true;
}

bool ArgParser::Push_value_in_arguments(Argument& found_arg, std::string& value) {
    if (found_arg.argument_type == "str") {
        found_arg.string_value.push_back(value);
        found_arg.is_added = true;
        if (found_arg.string_ptr) {
            *found_arg.string_ptr = value;
        } else if (found_arg.string_vector_ptr) {
            found_arg.string_vector_ptr->push_back(value);
        }
    } else if (found_arg.argument_type == "int") {
        if (!GetInt(value, found_arg.long_name)) {
            return false;
        } 
        found_arg.int_value.push_back(int_value);
        found_arg.is_added = true;
        if (found_arg.int_ptr) {
            *found_arg.int_ptr = int_value;
        } else if (found_arg.int_vector_ptr) {
            found_arg.int_vector_ptr->push_back(int_value);
        }
    }
    return true;
}