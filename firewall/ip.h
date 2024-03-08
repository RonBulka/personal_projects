#ifndef IP_H
#define IP_H

#include "generic-field.h"

/**
 *  IP class, subclass of GenericField.
 *  Represents an IP field. holds information about the IP address both
 *  masked and unmasked in its integer representation.
 */
class IP : public GenericField
{
    public:
        /**
         * @brief Check if the packet is accepted by the rule
         * @param packet the whole packet
         * @return true if the packet matches the rule (ip)
        */
        bool match(String packet);
        /**
         * @brief Set the value of the rule
         * @param value the value of the rule, src-ip or dst-ip parameter from packet
         * @return true if the value is valid, false otherwise
        */
        bool set_value(String value);

        ~IP() {}

        void set_ip_type(String type) { this->type = type; }

    private:
        // private fields
        String type;    // src-ip or dst-ip
        int full_ip;    // the full ip, before the masking
        int masked_ip;  // the masked ip
        int mask;       // the mask

        // private methods
        /**
         * @brief Check if the packet field matches the rule by checking the 
         *        field type and applying the rule mask if the field matches.
         * @param packet_field the packet field, one of the 4 packet params
         * @return true if the ip matches the rule with a rule mask, 
         * false otherwise
        */
        bool compare(String& packet_field);
};
#endif // IP_H