#include "VGATextDevice.hpp"
#include "IO.hpp"

namespace arch
{
  namespace i686
  {
    uint8_t* VGATextDevice::m_ScreenBuffer = (uint8_t*)0xB8000;
    VGATextDevice::VGATextDevice():
      m_ScreenX(0), m_ScreenY(0) {
      Clear();
    }

    size_t VGATextDevice::Read(uint8_t* data, size_t size) {
      return 0;
    }

    size_t VGATextDevice::Write(const uint8_t* data, size_t size) {
      for(size_t i = 0; i<size; i++){
        PutChar(data[i]);
      }
      return size;
    }

    void VGATextDevice::Clear(){
      for (int y = 0; y < ScreenHeight; y++)
      for (int x = 0; x < ScreenWidth; x++)
      {
        PutChar(x, y, '\0');
        PutColor(x, y, DefaultColor);
      }

      m_ScreenX = 0;
      m_ScreenY = 0;
      SetCursor(m_ScreenX, m_ScreenY);
    }


    void VGATextDevice::PutChar(int x, int y, char c)
    {
        m_ScreenBuffer[2 * (y * ScreenWidth + x)] = c;
    }

    void VGATextDevice::PutColor(int x, int y, uint8_t color)
    {
        m_ScreenBuffer[2 * (y * ScreenWidth + x) + 1] = color;
    }

    char VGATextDevice::GetChar(int x, int y)
    {
        return m_ScreenBuffer[2 * (y * ScreenWidth + x)];
    }

    uint8_t VGATextDevice::GetColor(int x, int y)
    {
        return m_ScreenBuffer[2 * (y * ScreenWidth + x) + 1];
    }

    void VGATextDevice::SetCursor(int x, int y)
    {
        int pos = y * ScreenWidth + x;

        OutB(0x3D4, 0x0F);
        OutB(0x3D5, (uint8_t)(pos & 0xFF));
        OutB(0x3D4, 0x0E);
        OutB(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    }

    void VGATextDevice::Scrollback(int lines)
    {
        for (int y = lines; y < ScreenHeight; y++)
            for (int x = 0; x < ScreenWidth; x++)
            {
              PutChar(x, y - lines, GetChar(x, y));
              PutColor(x, y - lines, GetColor(x, y));
            }

        for (int y = ScreenWidth - lines; y < ScreenWidth; y++)
            for (int x = 0; x < ScreenWidth; x++)
            {
              PutChar(x, y, '\0');
              PutColor(x, y, DefaultColor);
            }

        m_ScreenY -= lines;
    }

    void VGATextDevice::PutChar(char c){
      switch (c)
      {
          case '\n':
              m_ScreenX = 0;
              m_ScreenY++;
              break;

          case '\t':
              for (int i = 0; i < 4 - (m_ScreenX % 4); i++)
                PutChar(' ');
              break;

          case '\r':
              m_ScreenX = 0;
              break;

          default:
              PutChar(m_ScreenX, m_ScreenY, c);
              m_ScreenX++;
              break;
      }

      if (m_ScreenX >= ScreenWidth)
      {
          m_ScreenY++;
          m_ScreenX = 0;
      }
      if (m_ScreenY >= ScreenHeight)
          Scrollback(1);

      SetCursor(m_ScreenX, m_ScreenY);
    }

  }
}
