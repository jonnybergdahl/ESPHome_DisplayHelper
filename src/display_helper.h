class DisplayHelper
{
    public:

        static void renderFrame(DisplayBuffer* it, Font* time_font, esphome::homeassistant::HomeassistantTime* time)
        {
            it->filled_rectangle(0, it->get_height() - 20, it->get_width(), it->get_height() - 1, _ha_blue);
            it->strftime(it->get_width() /2, it->get_height(), time_font, TextAlign::BOTTOM_CENTER, "%H:%M", time->now());
        }

        static void renderCaption(DisplayBuffer* it, int index, Font* caption_font, const char* caption)
        {
            it->filled_rectangle(0, index * _line_height, it->get_width(), _line_height - 4, _ha_blue);
            it->print(it->get_width() / 2, index * _line_height + _line_height / 2 - 2, caption_font, TextAlign::CENTER, caption);
        }

#ifdef USE_TEXT_SENSOR
        static void renderLine(DisplayBuffer *it, int index, Font *font, TextSensor* sensor, TextSensor* name)
        {
            auto current_y = index * _line_height;
            
            if (sensor->has_state()) 
            {
                // ESP_LOGD("display_helper", "%s - %s", name->state.c_str(), sensor->state.c_str());
                if (sensor->state == "ok")
                {
                    it->filled_circle(8, current_y + 8, 8, _green);
                }
                else if (sensor->state == "problem")
                {
                    it->filled_circle(8, current_y + 8, 8, _red);
                }
                else 
                {
                    it->filled_circle(8, current_y + 8, 8, _blue);
                }
            }
            if (name->has_state()) 
            {
                it->printf(24, current_y, font, TextAlign::TOP_LEFT , "%s", name->state.c_str());
            }    
        }
#endif

#ifdef USE_TEXT_SENSOR
        static void renderRunningLine(DisplayBuffer *it, int index, Font *font, TextSensor* sensor, const char* text)
        {
            auto current_y = index * _line_height;
            
            if (sensor->has_state()) 
            {
                ESP_LOGD("display_helper", "%s - %s", text, sensor->state.c_str());
                if (sensor->state == "Running")
                {
                    it->filled_circle(8, current_y + 8, 8, _green);
                }
                else if (sensor->state == "Stopped")
                {
                    it->filled_circle(8, current_y + 8, 8, _red);
                }
                else 
                {
                    it->filled_circle(8, current_y + 8, 8, _blue);
                }
            }
            it->print(24, current_y, font, TextAlign::TOP_LEFT , text);
        }
#endif

#ifdef USE_TEXT_SENSOR
        static void renderWarningLine(DisplayBuffer *it, int index, Font *font, TextSensor* sensor, const char* text)
        {
            // Warnings are true if there is a problem, so on equals problem
            auto current_y = index * _line_height;
            
            if (sensor->has_state()) 
            {
                // ESP_LOGD("display_helper", "%s - %s", sensor->state.c_str(), text);
                if (sensor->state == "off")
                {
                    it->filled_circle(8, current_y + 8, 8, _green);
                }
                else if (sensor->state == "on")
                {
                    it->filled_circle(8, current_y + 8, 8, _red);
                }
                else 
                {
                    it->filled_circle(8, current_y + 8, 8, _blue);
                }
            }
            it->print(24, current_y, font, TextAlign::TOP_LEFT , text);
        }
#endif

#ifdef USE_TEXT_SENSOR
        static void renderVacuumStateLine(DisplayBuffer *it, int index, Font *font, TextSensor* vacuum, TextSensor* room)
        {
            // Warnings are true if there is a problem, so on equals problem
            auto current_y = index * _line_height;
            if (vacuum->has_state()) 
            {
                ESP_LOGD("display_helper", "room: %s", room->state.c_str());
                ESP_LOGD("display_helper", "vacuum: %s", vacuum->state.c_str());                
                
                std::string room_state = room->state;
                std::string room_name = "(Okänt rum)";
                std::string vacuum_state = vacuum->state.c_str();

                Color color = _green;
                // Check what different states there is
                // docked 
                if (vacuum->state == "Segment cleaning") 
                {
                    color = _yellow;
                    vacuum_state = "Städar";
                }
                else if (vacuum->state == "Charging")
                {
                    color = _blue;
                    vacuum_state = "Laddar";
                    room_state = "16";      // We know where you are!
                }
                else if (vacuum->state == "Returning home")
                {
                    color = _blue;
                    vacuum_state = "Återvänder";
                }
                else if (vacuum->state == "Emptying the bin")
                {
                    color = _blue;
                    vacuum_state = "Tömmer behållaren";
                }
                else if (vacuum->state == "Error")
                {
                    color = _red;
                    vacuum_state = "Fel";
                }
                else
                {
                    color = _red;
                    vacuum_state = vacuum->state;
                }

                if (_rooms.count(room_state) > 0)
                {
                    room_name = _rooms.at(room_state);
                }

                ESP_LOGD("display_helper", "room name: %s", room_name.c_str());
                it->filled_circle(8, current_y + 8, 8, color);
                it->printf(24, current_y, font, TextAlign::TOP_LEFT, "%s (%s)", room_name.c_str(), vacuum_state.c_str());
            }            
        }
#endif

#ifdef USE_SENSOR
        static void renderDurationLine(DisplayBuffer* it, int index, Font *font, Sensor* sensor, float maxHours, const char* text)
        {
            auto current_y = index * _line_height;
            
            if (sensor->has_state()) 
            {
                // ESP_LOGD("display_helper", "%s %f", text, sensor->state);
                int hours = int(sensor->state / 3600);
                int percentage = 100 * hours / maxHours;
                // ESP_LOGD("display_helper", "Hours: %i", text, hours);
                //ESP_LOGD("display_helper", "Percentage: %i", text, percentage);

                if (percentage <= 0)
                {
                    it->filled_circle(8, current_y + 8, 8, _red);
                }
                else if (percentage < 10)
                {
                    it->filled_circle(8, current_y + 8, 8, _yellow);
                }
                else
                {
                    it->filled_circle(8, current_y + 8, 8, _green);
                }                
                it->printf(24, current_y, font, TextAlign::TOP_LEFT, "%s (%i%%)", text, percentage);
            }
        }
#endif

#ifdef USE_SENSOR
        static void renderLine(DisplayBuffer* it, int index, Font *font, Sensor* sensor, const char* text)
        {
            auto current_y = index * _line_height;
            
            if (sensor->has_state()) 
            {
                ESP_LOGD("display_helper", "%s %f", text, sensor->state);

                if (sensor->state == 0)
                {
                    it->filled_circle(8, current_y + 8, 8, _red);
                }
                else
                {
                    it->filled_circle(8, current_y + 8, 8, _green);
                }                
                it->print(24, current_y, font, TextAlign::TOP_LEFT, text);
            }
        }
#endif

#ifdef USE_TEXT_SENSOR
        static void renderPlantLine(DisplayBuffer* it, int index, Font *font, Font* mdi_font, TextSensor* moisture, TextSensor* conductivity, TextSensor* name)
        {
            auto current_y = index * _line_height;
            
            if (moisture->has_state()) 
            {
                // ESP_LOGD("display_helper", "moisture %s", moisture->state);

                if (moisture->state == "ok")
                {
                    it->print(0, current_y, mdi_font, _green, TextAlign::TOP_LEFT, "\U000F058C");
                }
                else if (moisture->state == "Low")
                {
                    it->print(0, current_y, mdi_font, _red, TextAlign::TOP_LEFT, "\U000F058C");
                }
                else
                {
                    it->print(0, current_y, mdi_font, _blue, TextAlign::TOP_LEFT, "\U000F058D");
                }
            }                
            if (conductivity->has_state()) 
            {
                // ESP_LOGD("display_helper", "conductivity %s", conductivity->state);

                if (conductivity->state == "ok")
                {
                    it->print(20, current_y, mdi_font, _green, TextAlign::TOP_LEFT, "\U000F032A");
                }
                else if (conductivity->state == "Low")
                {
                    it->print(20, current_y, mdi_font, _red, TextAlign::TOP_LEFT, "\U000F032A");
                }
                else
                {
                    it->print(20, current_y, mdi_font, _blue, TextAlign::TOP_LEFT, "\U000F12D9");
                }                
            }
            it->printf(44, current_y, font, TextAlign::TOP_LEFT, name->state.c_str());
        }
#endif


#ifdef USE_BINARY_SENSOR
        static void renderBoolLine(DisplayBuffer* it, int index, Font *font, esphome::binary_sensor::BinarySensor* sensor, const char* text)
        {
            auto current_y = index * _line_height;
            
            if (sensor->has_state()) 
            {
                // ESP_LOGD("display_helper", "%s %i", text, sensor->state);
                if (sensor->state)
                {
                    it->filled_circle(8, current_y + 8, 8, _green);
                }
                else
                {
                    it->filled_circle(8, current_y + 8, 8, _red);
                }
                it->print(24, current_y, font, TextAlign::TOP_LEFT , text);
            }
        }
#endif

    private:
        DisplayHelper() {};
        static const int _line_height = 24;
        static const Color _red;
        static const Color _green;
        static const Color _blue;
        static const Color _yellow;
        static const Color _black;
        static const Color _ha_blue;
        static const std::map<std::string, std::string> _rooms;
};

const Color DisplayHelper::_red {255, 0, 0};
const Color DisplayHelper::_green {0, 255, 0};
const Color DisplayHelper::_blue {0, 0, 255};
const Color DisplayHelper::_yellow {255, 255, 0};
const Color DisplayHelper::_black {0, 0, 0};
const Color DisplayHelper::_ha_blue {65, 189, 245};
const std::map<std::string, std::string> DisplayHelper::_rooms {{ "16", "Sovrum" }, { "17", "Hall S" }, { "18", "Hall N" }, { "19", "Kök"}, { "20", "Kontor" }, { "21", "Vardagsrum" }};
