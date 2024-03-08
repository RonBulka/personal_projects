#ifndef PORT_H
#define PORT_H

#include "generic-field.h"

/**
 * @brief Port class - represents a port field, which can be a range of values.
 *        Port is a subclass of GenericField.
*/
class Port : public GenericField
{
    public:
        /**
         * @brief Check if the packet is accepted by the rule
         * @param packet the whole packet
         * @return true if the packet matches the rule (port)
        */
        bool match(String packet);

        /**
         * @brief Set the port rule
         * @param value the port rule
         * @return true if the port rule is valid, false otherwise
        */
        bool set_value(String value);

        ~Port() {}

        void set_port_type(String type) { this->type = type; }

    private:
        // private fields
        String type;        // src-port or dst-port
        int port_left;      // left part of the ports range
        int port_right;     // right part of the ports range
        bool flag;          // true if port is a range, false otherwise

        // private methods
        /**
         * @brief Check if the packet field matches the rule by checking the
         *        field type and and making sure that the port is within the
         *        allowed range.
         * @param packet_field the packet field, one of the 4 packet params
         * @return true if the port matches the rule, false otherwise
        */
        bool compare(String& packet_field);

};
#endif //PORT_H