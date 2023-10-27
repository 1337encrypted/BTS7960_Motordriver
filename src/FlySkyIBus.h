/*
 * Simple interface to the Fly Sky IBus RC system.
 * Based on the original work done by https://gitlab.com/timwilkinson/FlySkyIBus
 * I dont own this part of the code, thenks to Tim Wilkinson for his contribution.
 */

//some

#include <Arduino.h>
#include <inttypes.h>

class HardwareSerial;
class Stream;

class FlySkyIBus
{
    
private:
    enum State
    {
        GET_LENGTH,
        GET_DATA,
        GET_CHKSUML,
        GET_CHKSUMH,
        DISCARD,
    };

public:
    inline void begin(HardwareSerial&) __attribute__((always_inline));
    inline void begin(Stream&) __attribute__((always_inline));
    inline void loop(void) __attribute__((always_inline));
    inline uint16_t readChannel(uint8_t) __attribute__((always_inline));
    inline bool readSwitch(uint8_t) __attribute__((always_inline));



    static const uint8_t PROTOCOL_LENGTH = 0x20;
    static const uint8_t PROTOCOL_OVERHEAD = 3;       // <len><cmd><data....><chkl><chkh>
    static const uint8_t PROTOCOL_TIMEGAP = 3;        // Packets are received very ~7ms so use ~half that for the gap
    static const uint8_t PROTOCOL_CHANNELS = 10;
    static const uint8_t PROTOCOL_COMMAND40 = 0x40;   // Command is always 0x40

    uint8_t state;
    Stream* stream;
    uint32_t last;
    uint8_t buffer[PROTOCOL_LENGTH];
    uint8_t ptr;
    uint8_t len;
    uint16_t channel[PROTOCOL_CHANNELS];
    uint16_t chksum;
    uint8_t lchksum;
};

void FlySkyIBus::begin(HardwareSerial& serial)
{
    serial.begin(115200);
    begin((Stream&)serial);
}

void FlySkyIBus::begin(Stream& stream)
{
    this->stream = &stream;
    this->state = DISCARD;
    this->last = millis();
    this->ptr = 0;
    this->len = 0;
    this->chksum = 0;
    this->lchksum = 0;
}

void FlySkyIBus::loop(void)
{
    while (stream->available() > 0)
    {
        uint32_t now = millis();
        if (now - last >= PROTOCOL_TIMEGAP)
        {
            state = GET_LENGTH;
        }
        last = now;

        uint8_t v = stream->read();
        switch (state)
        {
            case GET_LENGTH:
            if (v <= PROTOCOL_LENGTH)
            {
                ptr = 0;
                len = v - PROTOCOL_OVERHEAD;
                chksum = 0xFFFF - v;
                state = GET_DATA;
            }
            else
            {
                state = DISCARD;
            }
            break;

            case GET_DATA:
            buffer[ptr++] = v;
            chksum -= v;
            if (ptr == len)
            {
                state = GET_CHKSUML;
            }
            break;

            case GET_CHKSUML:
            lchksum = v;
            state = GET_CHKSUMH;
            break;

            case GET_CHKSUMH:
            // Validate checksum
            if (chksum == (v << 8) + lchksum)
            {
                // Execute command - we only know command 0x40
                switch (buffer[0])
                {
                    case PROTOCOL_COMMAND40:
                    // Valid - extract channel data
                    for (uint8_t i = 1; i < PROTOCOL_CHANNELS * 2 + 1; i += 2)
                    {
                        channel[i / 2] = buffer[i] | (buffer[i + 1] << 8);
                    }
                    break;

                    default: break;
                }
            }
            state = DISCARD;
            break;

            case DISCARD:
            default: break;
        }
    }
}

uint16_t FlySkyIBus::readChannel(uint8_t channelNr)
{
    if (channelNr < PROTOCOL_CHANNELS)
    {
        if(channel[channelNr] < 1000 && channel[channelNr] > 0)
          return 1000;
        else if(channel[channelNr] > 2000)
          return 2000;
        else if(channel[channelNr] == 0)
          return 0;
        
        return channel[channelNr];  
    }
    else
    {
        return 0;
    }

}

bool FlySkyIBus::readSwitch(uint8_t channelNr)
{
    return channel[channelNr] > 1500 ? true : false;
}
