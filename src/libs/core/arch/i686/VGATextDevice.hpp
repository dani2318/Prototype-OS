#pragma once
#include <dev/CharacterDevice.hpp>
#include <stdint.h>
#include "IO.hpp"
namespace arch
{
  namespace i686
  {
    static constexpr unsigned ScreenWidth = 80;
    static constexpr unsigned ScreenHeight = 25;
    static constexpr uint8_t DefaultColor = 0x7;

    class VGATextDevice: public CharacterDevice{
      public:
        VGATextDevice();
        ~VGATextDevice() {};
        virtual size_t Read(uint8_t* data, size_t size);
        virtual size_t Write(const uint8_t* data, size_t size);

        void Clear();
      private:
        void PutChar(char c);
        void PutChar(int x, int y, char c);
        void PutColor(int x, int y, uint8_t color);

        uint8_t GetColor(int x, int y);
        char GetChar(int x, int y);

        void SetCursor(int x, int y);
        void Scrollback(int lines);
        static uint8_t* m_ScreenBuffer;
        int m_ScreenX, m_ScreenY;
    };

  }
}
