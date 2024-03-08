#include "port.h"

int absolute(int x);
bool validate(String& left, int& port);
bool parse_port(String& str, int& port);
bool split_port (String& port, String& left, String& right, bool& flag);
bool port_split_type(String& value, String& type, String& port);


bool Port::match(String packet)
{
    bool flag = false;
    size_t split_size = 0;
    // split the packet to the 4 parameters
    packet.split(",", nullptr, &split_size);
    if ((split_size != 1) && (split_size != 4))
    {
        return false;
    }
    String* initial_split = nullptr;
    packet.split(",", &initial_split, &split_size);
    String packet_field = String();
    // check if the packet matches the rule
    for (size_t i=0; i < split_size; i++){
        packet_field = initial_split[i].trim();
        if (compare(packet_field)){
            flag = true;
            break;
        }
    }
    delete[] initial_split;
    return flag;
}


bool Port::compare(String& packet_field){
    int packet_port_left = 0;
    int packet_port_right = 0;
    bool packet_flag = false;
    String string_left;
    String string_right;
    String packet_type;
    String port;
    
    // split the packet field to type and port
    if(!port_split_type(packet_field, packet_type, port))
    {
        return false;
    }
    // check if the type is port
    if (!packet_type.equals(type)){
        return false;
    }
    // split the port to port range
    if (!(split_port(port, string_left, string_right, packet_flag))){
        return false;
    }
    // parse the port
    if (!parse_port(string_left, packet_port_left)){
        return false;
    }
    // parse the port range if there is one
    if (packet_flag && !parse_port(string_right, packet_port_right)){
        return false;
    }
    // if there is no port range, set the right port to be the left port
    else{
        packet_port_right = packet_port_left;
    }
    
    if ((packet_port_left >= port_left) && (packet_port_right <= port_right)){
        return true;
    }
    
    return false;
}


bool Port::set_value(String value)
{
    String string_left;
    String string_right;
    // split the port range to lower and upper bound
    if (!split_port(value, string_left, string_right, flag)){
        return false;
    }

    // parse the port range
    if (!parse_port(string_left, port_left)){
        return false;
    }
    if (flag && !parse_port(string_right, port_right)){
        return false;
    }
    else if (!flag) {
        port_right = port_left;
    }
    return true;
}

/**
 * @brief parses the port to an integer
 * @param str the port as a string object
 * @param port write to param for the port as an integer
 * @return true if the parsing was successful, false otherwise
*/
bool parse_port(String& str, int& port){
    if (!validate(str, port)){
        return false;
    }
    return true;
}

/**
 * @brief validates that the port has a valid value
 * @param str the port as a string object
 * @param port write to param for the port as an integer
 * @return true if the validation was successful, false otherwise
*/
bool validate(String& str, int& port){
    int temp = str.to_integer();
    if (temp > 65535 || temp < 0){
        return false;
    }
    port = temp;
    return true;
}

/**
 * @brief Split the packet field to type and port
 * @param value the packet field, one of the 4 packet params
 * @param type write to param for the type
 * @param port write to param for the port
 * @return true if the split was successful, false otherwise
*/
bool port_split_type(String& value, String& type, String& port){
    size_t size_split = 0;
    (value).split("=", nullptr, &size_split);
    if (size_split != 2)
    {
        return false;
    }
    String* value_split = nullptr;
    (value).split("=", &value_split, &size_split);
    type = value_split[0].trim();
    port = value_split[1].trim();
    delete[] value_split;
    return true;
}

/**
 * @brief splits the port by "-" to get the range of the port
 * @param port the ip address with the mask (if there is one)
 * @param left write to param for the ip address
 * @param right write to param for the mask
 * @param flag write to param for the flag, true if the port is a range
 * @return true if the split was successful, false otherwise
*/
bool split_port(String& port,
            String& left,
            String& right,
            bool& flag)
{
    size_t size_split = 0;
    port.split("-", nullptr, &size_split);
    
    if (size_split > 2)
    {
        return false;
    }

    String* initial_split = nullptr;
    port.split("-",&initial_split, &size_split);

    if (size_split == 1){
        left = initial_split[0].trim();
        flag = false;
    }
    else{
        left = initial_split[0].trim();
        right = initial_split[1].trim();
        flag = true;
    }

    delete[] initial_split;
    return true;
}

/**
 * @brief returns the absolute value of x
 * @param x the number
 * @return the absolute value of x
 * @note we werent sure if we should include math headers, so we wrote our own
*/
int absolute(int x){
    if (x < 0){
        return -x;
    }
    return x;
}