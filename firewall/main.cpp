#include "ip.h"
#include "port.h"
#include "input.h"

bool is_arg_ip(String &type);
bool rule_split_type(String& rule, String& type, String& value);

/**
 * @brief check if the arguments are valid packets, afterwards parse the stdin
 *        and leave only packets that match the given rule.
 * @param argc the number of arguments
 * @param argv the arguments
 * @return 0 if the arguments are valid, 1 otherwise
*/
int main(int argc, char **argv)
{
    if (check_args(argc, argv) != 0)
    {
        return 1;
    }
    
    String arg = String(argv[1]);
    String type;
    String value;
    if (!rule_split_type(arg, type, value)){
        return 1;
    }
    if (is_arg_ip(type))
    {
        IP ip;
        ip.set_ip_type(type);
        ip.set_value(value);
        parse_input(ip);
    }
    else
    {
        Port port;
        port.set_port_type(type);
        port.set_value(value);
        parse_input(port);
    }
    return 0;
}

/**
 * @brief Split the rule field to type and port
 * @param rule the packet field, one of the 4 packet params
 * @param type write to param for the type
 * @param value write to param for the value
 * @return true if the split was successful, false otherwise
*/
bool rule_split_type(String& rule, String& type, String& value){
    size_t size_split = 0;
    (rule).split("=", nullptr, &size_split);
    if (size_split != 2)
    {
        return false;
    }
    String* rule_split = nullptr;
    (rule).split("=", &rule_split, &size_split);
    type = rule_split[0].trim();
    value = rule_split[1].trim();
    delete[] rule_split;
    return true;
}


/**
 * @brief check if the String represents an ip address
 * @param arg the String
 * @return true if the String represents an ip, false otherwise
*/
bool is_arg_ip(String &type){
    size_t size_split = 0;

    (type).split("ip", nullptr, &size_split);
    if (size_split != 2)
    {
        return false;
    }

    return true;
}