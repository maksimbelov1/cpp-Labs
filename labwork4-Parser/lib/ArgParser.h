#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

namespace ArgumentParser { 
    struct Argument {
        std::string argument_type;
        char short_name;
        std::string long_name;
        std::string description;

        std::vector<std::string> string_value;
        std::vector<int32_t> int_value;
        std::vector<bool> bool_value;

        std::string* string_ptr = nullptr; 
        int32_t* int_ptr = nullptr; 
        bool* bool_ptr = nullptr; 
        std::vector<std::string>* string_vector_ptr = nullptr;
        std::vector<int32_t>* int_vector_ptr = nullptr;
        std::vector<bool>* bool_vector_ptr = nullptr;

        bool is_added;
        int32_t multi_value;
        bool is_multi_value;
        bool is_positional;


        Argument(const std::string argument_type_, const char short_name_, const std::string& long_name_, const std::string& description_):
        argument_type(argument_type_),
        short_name(short_name_),
        long_name(long_name_),
        description(description_),
        is_added(false),
        multi_value(0),
        is_multi_value(false),
        is_positional(false) {}    
    };

class ArgParser {
    public:
        ArgParser(const std::string& parser_name);
        
        ArgParser& AddStringArgument(const char short_name, const std::string& long_name, const std::string& description = "");
        ArgParser& AddStringArgument(const std::string& long_name, const std::string& description = "");
       
        ArgParser& AddIntArgument(const char short_name, const std::string& long_name, const std::string& description = "");
        ArgParser& AddIntArgument(const std::string& long_name, const std::string& description = "");

        ArgParser& AddFlag(const char short_name, const std::string& long_name, const std::string& description = "");
        ArgParser& AddFlag(const std::string& long_name, const std::string& description = "");

        ArgParser& Default(const char* value);
        ArgParser& Default(const int32_t value);
        ArgParser& Default(const bool value);

        ArgParser& StoreValue(std::string& value_name);
        ArgParser& StoreValue(int32_t& value_name);
        ArgParser& StoreValue(bool& value_name);

        ArgParser& StoreValues(std::vector<std::string>& values_name);
        ArgParser& StoreValues(std::vector<int32_t>& values_name);
        ArgParser& StoreValues(std::vector<bool>& values_name);

        ArgParser& MultiValue(size_t min_number_of_arguments = 0);

        ArgParser& Positional();

        bool Parse(const std::vector<std::string>& args);
        bool Parse(int argc, char** argv);

        std::string GetStringValue(const std::string& name, const int32_t index = 0);
        int32_t GetIntValue(const std::string& name, const int32_t index = 0);
        bool GetFlag(const std::string& name, const int32_t index = 0);

        void AddHelp(const char short_name, const std::string& long_name, const std::string& description);
        bool Help();
        void HelpDescription();

        bool GetInt(std::string& value, std::string& name);
        bool is_parsing_successful();
        bool Push_value_in_arguments(Argument& found_arg, std::string& value);
        
    private:
        bool is_help_added = false;
        std::string help_long_name = "";
        char help_short_name = ' ';
        std::string help_information = "Some Description about program:\n\n";
        std::string parser_name_ = "My Parser";
        std::string last_add_argument = "";
        std::string last_parsed_argument_long_name = "#";
        char last_parsed_argument_short_name = '#';
        int32_t int_value = 0;
        std::string value = "";
        bool found = false;
        std::vector<Argument> arguments;
};

} 