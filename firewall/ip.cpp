#include "ip.h"

bool validate_byte(int value);
bool validate_mask(int value);
bool parse_ip(String& string_ip, int& ip);
bool parse_mask(String& string_mask, int& mask);
bool ip_split_type(String& packet_field, String& type_output, String& str_ip);
bool split_ip(String& packet, String& ip_output, String& mask_output);
int keepFirstBits(int y, int x);


bool IP::match(String packet)
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


bool IP::compare(String& packet_field){
    int packet_ip = 0;
    int packet_mask = 0;
    int packet_masket_ip = 0;
    String str_ip;
    String str_full_ip;
    String str_mask;
    String packet_type;

    // split the packet field to type and ip
    if(!ip_split_type(packet_field, packet_type, str_ip))
    {
        return false;
    }
    // check if the type is ip
    if (!packet_type.equals(type)){
        return false;
    }
    // split the ip to ip and mask
    if(!split_ip(str_ip, str_full_ip, str_mask))
    {
        return false;
    }
    // parse the ip and mask to integers
    if(!parse_ip(str_full_ip, packet_ip))
    {
        return false;
    }
    if(!parse_mask(str_mask, packet_mask)){
        return false;
    }
    // apply the rule mask
    packet_masket_ip = keepFirstBits(packet_ip, mask);
    // check if the masked ip matches the rule masked ip
    bool return_val = (packet_masket_ip == masked_ip);
    return return_val;
}


bool IP::set_value(String value)
{
    String str_full_ip;
    String str_mask;
    // split the ip to ip and mask
    if(!split_ip(value, str_full_ip, str_mask))
    {
        return false;
    }
    // parse the ip and mask to integers
    if(!parse_ip(str_full_ip, full_ip))
    {
        return false;
    }
    if(!parse_mask(str_mask, mask)){
        return false;
    }
    // apply the rule mask
    masked_ip = keepFirstBits(full_ip, mask);

    return true;
}


/**
 * @brief Split the packet field to type and ip
 * @param packet_field the packet field, one of the 4 packet params
 * @param type_output write to param for the type of the packet field
 * @param str_ip write to param for the ip of the packet field
 * @return true if the split was successful, false otherwise
*/
bool ip_split_type(String& packet_field, String& type_output, String& str_ip){
    size_t size_split = 0;
    (packet_field).split("=", nullptr, &size_split);
    if (size_split != 2)
    {
        return false;
    }
    String* packet_split = nullptr;
    (packet_field).split("=", &packet_split, &size_split);
    type_output = packet_split[0].trim();
    str_ip = packet_split[1].trim();
    delete[] packet_split;
    return true;
}

/**
 * @brief splits the ip address by "/" to get the mask and the ip address
 * @param str_ip the ip address with the mask (if there is one)
 * @param ip_output write to param for the ip address
 * @param str_mask write to param for the mask
 * @return true if the split was successful, false otherwise
*/
bool split_ip(String& str_ip, String& ip_output, String& mask_output){
    size_t size_split = 0;
    (str_ip).split("/", nullptr, &size_split);
    if (size_split > 2)
    {
        return false;
    }

    String* initial_split = nullptr;
    (str_ip).split("/", &initial_split, &size_split);

    if (size_split == 1){
        ip_output = initial_split[0].trim();
        mask_output = String("0");
    }
    else{        
        ip_output = initial_split[0].trim();
        mask_output = initial_split[1].trim();
    }

    delete[] initial_split;
    return true;
}

/**
 * @brief parses the ip address to an integer
 * @param value the ip address as a string object
 * @param ip_output write to param for the ip address as an integer
 * @return true if the parsing was successful, false otherwise
*/
bool parse_ip(String& value, int& ip_output){
    size_t ip_size = 0;
    (value).split(".", nullptr, &ip_size);
    if (ip_size != 4)
    {
        return false;
    }

    String* ip = nullptr;
    (value).split(".", &ip, &ip_size);
    int* arr = new int[ip_size];

    for (size_t i = 0; i < ip_size; i++)
    {
        int byte_temp = ip[i].to_integer();
        if (validate_byte(byte_temp)){
            arr[i] = byte_temp;
        }
        else{
            delete[] ip;
            delete[] arr;
            return false;
        }
    }

    ip_output = (arr[0] << 24) | (arr[1] << 16) | (arr[2] << 8) | arr[3];

    delete[] arr;
    delete[] ip;
    return true;
}

/**
 * @brief parses the mask to an integer
 * @param value the mask as a string object
 * @param ip_output write to param for the mask as an integer
 * @return true if the parsing was successful, false otherwise
*/
bool parse_mask(String& value, int& mask_output){
    int mask = (value).to_integer();
    if (validate_mask(mask))
    {
        mask_output = mask;
        return true;
    }
    return false;
}

/**
 * @brief validates that the value is a valid byte
 * @param value the value to validate
 * @return true if the value is a valid byte, false otherwise
*/
bool validate_byte(int value){
    if (value < 0 || value > 255)
    {
        return false;
    }
    return true;
}

/**
 * @brief validates that the value is a valid mask
 * @param value the value to validate
 * @return true if the value is a valid mask, false otherwise
*/
bool validate_mask(int value){
    if (value < 0 || value > 32)
    {
        return false;
    }
    return true;
}

/**
 * @brief keeps the first x bits of the integer y
 * @param y the integer to keep the bits from
 * @param x the number of bits to keep
 * @return the integer with only the first x bits
*/
int keepFirstBits(int y, int x) {
    // Create a mask with 1s in the first x bits from the MSB
    int mask = ~((1 << (sizeof(y) * 8 - x)) - 1); 
    // Perform bitwise AND operation to keep only the desired bits
    return y & mask; 
}