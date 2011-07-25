#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#include "Base.h"
#include "Config.h"

#include <signal.h>

#if _d_os_win
  #include <windows.h>
  #include "resource.h"
#endif

#include <cmath>

//
//
//
class Log
{
  public:
    Log()
    {
      used = 0;
    }

    ~Log()
    {
      printf("%s\n", buffer);
    }

    Log& operator <<(bool arg)
    {
      print(arg ? "true" : "false");
      
      return *this;
    }

    Log& operator <<(int8 arg)
    {
      print("%d", arg);

      return *this;
    }

    Log& operator <<(uint8 arg)
    {
      print("%d", arg);

      return *this;
    }

    Log& operator <<(int16 arg)
    {
      print("%d", arg);

      return *this;
    }

    Log& operator <<(uint16 arg)
    {
      print("%d", arg);

      return *this;
    }

    Log& operator <<(int32 arg)
    {
      print("%d", arg);

      return *this;
    }

    Log& operator <<(uint32 arg)
    {
      print("%d", arg);

      return *this;
    }

    Log& operator <<(int64 arg)
    {
      print("%d", arg);

      return *this;
    }

    Log& operator <<(uint64 arg)
    {
      print("%d", arg);

      return *this;
    }

    Log& operator <<(float64 arg)
    {
      print("%f", arg);

      return *this;
    }

    Log& operator <<(const char *arg)
    {
      print(arg);

      return *this;
    }

    void print(const char *fmt, ...)
    {
      va_list args;
      va_start(args, fmt);

      #if _d_cc_msc && _d_cc_msc_major > 8
        used += ::vsprintf_s(&buffer[used], _d_log_buffer_length - used, fmt, args);
      #else
        char b[_d_log_buffer_length];
        int c = vsprintf(b, fmt, args);

        if(c > _d_log_buffer_length - used)
          b[c] = '\0';

        used += sprintf(&buffer[used], b);
      #endif

      va_end(args);
    }

    const char* GetBuffer() const
    {
      return buffer;
    }

  private:
    char buffer[_d_log_buffer_length];
    int used;
};

#if _d_enable_log_info
  #define _d_log_info(__args) \
    { \
      Log() << "Info: " << __args; \
    }
#endif

#if _d_enable_log_warn
  #define _d_log_warn(__args) \
    { \
      Log() << "Warn: " << __args; \
    }
#endif

#if _d_enable_log_err
  #define _d_log_err(__args) \
    { \
      Log() << "Err: " << __args; \
    }
#endif

#if _d_os_win
  #define _d_log_fatal(__args) \
    { \
      Log l; \
      l << "Fatal: " << __args; \
      MessageBoxA( \
        null, \
        l.GetBuffer(), \
        "Fatal", \
        MB_ICONERROR | MB_OK | MB_DEFBUTTON1); \
      ::exit(1); \
    }
#else
  #define _d_log_fatal(__args) \
    { \
      Log() << "Fatal: " << __args; \
      ::exit(1); \
    }
#endif

//
//
//
template<typename T> class Vector
{
  public:
    T x;
    T y;

    Vector()
      : x(0), y(0)
    {
      ;
    }

    Vector(T x, T y)
      : x(x), y(y)
    {
      ;
    }

    Vector(const Vector<T> &v)
      : x(v.GetX()), y(v.GetY())
    {
      ;
    }

    T GetX() const
    {
      return x;
    }

    T GetY() const
    {
      return y;
    }

    Vector<T>& Set(T x, T y)
    {
      this->x = x;
      this->y = y;

      return *this;
    }

    Vector<T>& Set(const Vector<T> &v)
    {
      x = v.GetX();
      y = v.GetY();

      return *this;
    }

    T GetDistance(const Vector<T> &to) const
    {
      return ::sqrt(GetDistanceSquared(to));
    }

    T GetDistanceSquared(const Vector<T> &to) const
    {
      T dx, dy;
      dx = x - to.GetX();;
      dy = y - to.GetY();

      return (dx * dx) + (dy * dy);
    }

    T GetLength() const
    {
      return ::sqrt(GetLengthSquared());
    }

    T GetLengthSquared() const
    {
      return (x * x) + (y * y);
    }

    Vector<T>& Add(const Vector<T> &v)
    {
      Set(x + v.GetX(), y + v.GetY());

      return *this;
    }

    Vector<T>& Sub(const Vector<T> &v)
    {
      Set(x - v.GetX(), y - v.GetY());

      return *this;
    }

    Vector<T>& Mul(const T to)
    {
      Set(x * to, y * to);

      return *this;
    }

    Vector<T>& Mul(const Vector<T> &v)
    {
      Set(x * v.GetX(), y * v.GetY());

      return *this;
    }
};

typedef Vector<GLdouble> Vector2d;

//
//
//
template<typename T> class Line
{
  public:
    Vector<T> origin;
    Vector<T> direction;

    static Line<T> Make(const Vector<T> &begin, const Vector<T> &end)
    {
      Vector<T> origin(begin);
      Vector<T> direction(end);
      direction.Sub(begin);

      return Line(origin, direction);
    }

    Line()
    {
      ;
    }

    Line(const Vector<T> &origin, const Vector<T> &direction)
    {
      this->origin.Set(origin);
      this->direction.Set(direction);
    }

    Line(const Line<T> &copy)
    {
      this->origin.Set(copy.GetOrigin());
      this->direction.Set(copy.GetDirection());
    }

    Vector<T> GetOrigin() const
    {
      return origin;
    }

    Vector<T> GetDirection() const
    {
      return direction;
    }

    T GetLength() const
    {
      return direction.GetLength();
    }

    Line<T>& AddLength(T length)
    {
      if(length != 0)
      {
        length = T(1) + (length / GetLength());
        direction.Set(direction.x * length, direction.y * length);
      }

      return *this;
    }

    Line<T>& SetLength(T length)
    {
      const T old = GetLength();
      length = length - old;
      if(length == 0)
        return *this;

      length = T(1) + (length / old);
      direction.Set(direction.x * length, direction.y * length);

      return *this;
    }

    Vector<T> GetAbsDirection()
    {
      Vector<T> v(origin);
      v.Add(direction);

      return v;
    }
};

typedef Line<GLdouble> Line2d;

//
//
//
class TimeMgr
{
  public:
    typedef uint32 Time;

    static Time GetTicks()
    {
      // "Initialization". Taking in account application loading time,
      // as SDL_GetTicks() returns time since SDL init.
      static Time t = SDL_GetTicks();

      return SDL_GetTicks() - t;
    }
};

//
//
//
typedef GLuint GlTexture;

class Texture
{
  public:
    const GlTexture TEXTURE;

    const GLint WIDTH;
    const GLint HEIGHT;

    const GLdouble TEXEL_WIDTH;
    const GLdouble TEXEL_HEIGHT;

    Texture(GlTexture texture, GLint width, GLint height, GLdouble texelWidth, GLdouble texelHeight)
      : TEXTURE(texture), WIDTH(width), HEIGHT(height), TEXEL_WIDTH(texelWidth), TEXEL_HEIGHT(texelHeight)
    {
      ;
    }

    ~Texture()
    {
      glDeleteTextures(1, &TEXTURE);
    }

    operator GlTexture()
    {
      return TEXTURE;
    }

    void DrawQuad(GLdouble x, GLdouble y)
    {
      glBindTexture(GL_TEXTURE_2D, TEXTURE);
      glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex3d(x, y, 0);
 
        glTexCoord2d(TEXEL_WIDTH, 0);
        glVertex3d(WIDTH + x, y, 0);
 
        glTexCoord2d(TEXEL_WIDTH, TEXEL_HEIGHT);
        glVertex3d(WIDTH + x, HEIGHT + y, 0);
 
        glTexCoord2d(0, TEXEL_HEIGHT);
        glVertex3d(x, HEIGHT + y, 0);
      glEnd();
    }
};

//
//
//
class Fade
{
  public:
    const uint32 FADE_MILLIS;
    const uint32 SLEEP_MILLIS;

    Fade()
      : FADE_MILLIS(0), SLEEP_MILLIS(0)
    {
      ;
    }

    Fade(uint32 fadeMillis, uint32 sleepMillis = 0, bool fadeIn = true)
      : FADE_MILLIS(fadeMillis), SLEEP_MILLIS(sleepMillis),
        fadeIn(true), fade(fadeIn ? 0 : 1), sleep(0)
    {
      ;
    }

    //Fade(uint32 fadeMillis, uint32 sleepMillis = 0, bool fadeIn = true, float64 fade = 0)
    //  : FADE_MILLIS(fadeMillis), SLEEP_MILLIS(sleepMillis),
    //    fadeIn(true), fade(fadeIn ? 0 : 1), sleep(0)
    //{
    //  ;
    //}

    float64 Calc(uint32 prevTime, uint32 currentTime)
    {
      const uint32 elapsedTime = currentTime - prevTime;

      sleep += elapsedTime;
      if(sleep >= SLEEP_MILLIS)
      {
        float64 fadeTick = (double)elapsedTime / (double)FADE_MILLIS;
        if(fadeTick > FADE_MILLIS)
        {
          // TODO: !
        }

        fade += fadeIn ? fadeTick : -fadeTick;
        if(fade >= 1)
        {
          fadeIn = false;

          if(fade > 1)
            fade += 1 - fade;
        }
        elif(fade <= 0)
        {
          fadeIn = true;
          sleep = 0;

          if(fade < 0)
            fade = -fade;
        }

        if(fade < 0 || fade > 1)
          _d_log_fatal("Fade: " << fade);

        return fade;
      }
    }

  private:
    bool fadeIn;
    float64 fade;
    uint32 sleep;
};

//
//
//
class NightCityEntity
{
  public:
    Vector2d pos;
    Texture &texture;
    Fade fade;

    NightCityEntity(const Vector2d &position, Texture &texture)
      : pos(position), texture(texture)
    {
      ;
    }

    NightCityEntity(const Vector2d &position, Texture &texture, Fade &fade)
      : pos(position), texture(texture), fade(fade)
    {
      ;
    }
};

//
//
//
class AirplaneEntity
{
  public:
    Line2d path;

    Texture &airplaneTexture;

    Texture &lightsRedTexture;
    Texture &lightsGreenTexture;
    Texture &lightsWhiteTexture;

    Fade fade;

    TimeMgr::Time time;

    AirplaneEntity(const Line2d &path, Texture &airplaneTexture,
      Texture &lightsRedTexture, Texture &lightsGreenTexture, Texture &lightsWhiteTexture,
      Fade &fade)
      : path(path), airplaneTexture(airplaneTexture),
        lightsRedTexture(lightsRedTexture), lightsGreenTexture(lightsGreenTexture), lightsWhiteTexture(lightsWhiteTexture),
        fade(fade),
        time(0)
    {
      ;
    }
};

//
//
//
class App
{
  public:
    const uint32 SCREEN_WIDTH;
    const uint32 SCREEN_HEIGHT;
    const uint32 SOUND_VOLUME;
    const char *const WINDOW_CAPTION;

    App(uint32 screenWidth, uint32 screenHeight, uint32 soundVolume, char *windowCaption)
      : SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight), SOUND_VOLUME(soundVolume), WINDOW_CAPTION(windowCaption)
    {
      screen = null;

      nowPlayingTexture = null;
      
      nightCityTexture = null;
      nightCityLights1Texture = null;

      airplaneTexture = null;
      airplaneLightsRedTexture = null;
      airplaneLightsGreenTexture = null;
      airplaneLightsWhiteTexture = null;

      nowPlaying = null;
      
      nightCity = null;
      nightCityLights1 = null;

      airplane = null;

      blues = null;
    }

    void Init()
    {
      //
      if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        _d_log_fatal("Failed to initialize SDL: " << SDL_GetError());

      //
      atexit(SDL_Quit);

      //
      SDL_WM_SetCaption(WINDOW_CAPTION, WINDOW_CAPTION);

      //
      if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) < 0)
        _d_log_fatal("Failed to initialize audio: " << SDL_GetError());

      //
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
      screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_OPENGL/*SDL_DOUBLEBUF | SDL_HWPALETTE | SDL_HWSURFACE*/);
      if(!screen)
        _d_log_fatal("Failed to initialize video: " << SDL_GetError());

      //
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
 
      glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
 
      glClear(GL_COLOR_BUFFER_BIT);
 
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
 
      glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
 
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //
      SDL_RWops *rw = LoadResource(_d_app_res_blues);
      if(!rw)
        _d_log_fatal(_d_file_line << ": " << SDL_GetError());
      //blues = Mix_LoadMUS("blues.mp3");
      blues = Mix_LoadMUS_RW(rw);
      if(!blues)
        _d_log_fatal(_d_file_line << ": " << SDL_GetError());
      //SDL_FreeRW(rw); // Commented out. Required by streamer.

      //
      #define _d_load_img(__id, __dest) \
        { \
          SDL_RWops *rw = LoadResource(__id); \
          if(!rw) \
            _d_log_fatal("!rw: " << ": " << SDL_GetError()); \
          \
          SDL_Surface *tmp = IMG_Load_RW(rw, 0); \
          if(!tmp) \
            _d_log_fatal("!tmp: " << ": " << SDL_GetError()); \
          \
          __dest = MakeTexture(tmp); \
          if(!__dest) \
            _d_log_fatal("!__dest"); \
          SDL_FreeSurface(tmp); \
          SDL_FreeRW(rw); \
        }

      _d_load_img(_d_app_res_np, nowPlayingTexture);

      _d_load_img(_d_app_res_nc, nightCityTexture);
      _d_load_img(_d_app_res_nc_lights_1, nightCityLights1Texture);

      _d_load_img(_d_app_res_airplane, airplaneTexture);
      _d_load_img(_d_app_res_airplane_lights_red, airplaneLightsRedTexture);
      _d_load_img(_d_app_res_airplane_lights_green, airplaneLightsGreenTexture);
      _d_load_img(_d_app_res_airplane_lights_white, airplaneLightsWhiteTexture);

      //
      nowPlaying = new NightCityEntity(Vector2d(_d_app_np_x, _d_app_np_y), *nowPlayingTexture, Fade(_d_app_np_fade));

      nightCity = new NightCityEntity(Vector2d(), *nightCityTexture);
      nightCityLights1 = new NightCityEntity(Vector2d(), *nightCityLights1Texture, Fade(_d_app_nc_lights_1_fade));

      airplane = new AirplaneEntity(
        Line2d::Make(
          // Reversed.
          Vector2d(_d_app_airplane_landing_to_x, _d_app_airplane_landing_to_y),
          Vector2d(_d_app_airplane_landing_from_x, _d_app_airplane_landing_from_y)),
        *airplaneTexture,
        *airplaneLightsRedTexture, *airplaneLightsGreenTexture, *airplaneLightsWhiteTexture,
        Fade(_d_app_airplane_lights_fade, _d_app_airplane_lights_sleep));
    }

    void Run()
    {
      //
      if(Mix_PlayMusic(blues, -1) == -1)
        _d_log_fatal("Mix_PlayMusic(): " << SDL_GetError());

      //
      TimeMgr::Time prevTime = TimeMgr::GetTicks();
      {
        airplane->time = prevTime;
      }

      forever
      {
        SDL_Event e; 
        while(SDL_PollEvent(&e))
        {
          if(e.type == SDL_QUIT)
            return;
        }

        //
        //static TimeMgr::Time prevTime = TimeMgr::GetTicks();
        const TimeMgr::Time currentTime = TimeMgr::GetTicks();
        const TimeMgr::Time elapsedTime = currentTime - prevTime;

        // Airplane position.
        {
          GLdouble pathLength = airplane->path.GetLength();
          if(pathLength <= 0)
          {
            airplane->path = Line2d::Make(
              // Reversed.
              Vector2d(_d_app_airplane_landing_to_x, _d_app_airplane_landing_to_y),
              Vector2d(_d_app_airplane_landing_from_x, _d_app_airplane_landing_from_y));

            pathLength = airplane->path.GetLength();

            airplane->time = currentTime;
          }

          uint32 elapsedTime = currentTime - airplane->time;
          GLdouble distance = _d_app_airplane_landing_speed * elapsedTime;
          airplane->path.AddLength(pathLength < distance ? -pathLength : -distance);

          airplane->time = currentTime;
        }

        //
        if(SDL_MUSTLOCK(screen))
          SDL_LockSurface(screen);
        glClear(GL_COLOR_BUFFER_BIT);

        //
        nightCity->texture.DrawQuad(nightCity->pos.GetX(), nightCity->pos.GetY());

        float64 nightCityFade = nightCityLights1->fade.Calc(prevTime, currentTime);
        glColor4d(nightCityFade, 0.055, 0.055, 1);
        nightCityLights1->texture.DrawQuad(nightCityLights1->pos.GetX(), nightCityLights1->pos.GetY());
        glColor4d(1, 1, 1, 1);

        //
        GLdouble airplaneX = airplane->path.GetAbsDirection().GetX();
        GLdouble airplaneY = airplane->path.GetAbsDirection().GetY();

        airplane->airplaneTexture.DrawQuad(airplaneX, airplaneY);

        float64 airplaneLightsfade = airplane->fade.Calc(prevTime, currentTime);

        glColor4d(airplaneLightsfade, 0, 0, airplaneLightsfade);
        airplane->lightsRedTexture.DrawQuad(airplaneX, airplaneY);

        glColor4d(0, airplaneLightsfade, 0, airplaneLightsfade);
        airplane->lightsGreenTexture.DrawQuad(airplaneX, airplaneY);

        glColor4d(airplaneLightsfade, airplaneLightsfade, airplaneLightsfade, airplaneLightsfade);
        airplane->lightsWhiteTexture.DrawQuad(airplaneX, airplaneY);

        glColor4d(1, 1, 1, 1);

        //
        if(currentTime >= _d_app_np_appear && currentTime <= (_d_app_np_appear + (_d_app_np_fade * 2) + _d_app_np_sleep))
        {
          float64 npFade;
          if(currentTime >= _d_app_np_appear + _d_app_np_fade && currentTime <= _d_app_np_appear + _d_app_np_fade + _d_app_np_sleep)
            npFade = 1;
          else
            npFade = nowPlaying->fade.Calc(prevTime, currentTime);

          glColor4d(1, 1, 1, npFade);
          nowPlaying->texture.DrawQuad(_d_app_np_x, _d_app_np_y);

          glColor4d(1, 1, 1, 1);
        }

        //
        if(SDL_MUSTLOCK(screen))
          SDL_FreeSurface(screen);
        SDL_GL_SwapBuffers();
        SDL_Delay(1);

        //
        prevTime = currentTime;
      }
    }

    void Destroy()
    {
      delete nowPlaying;

      delete nightCity;
      delete nightCityLights1;

      delete airplane;

      delete nowPlayingTexture;

      delete nightCityTexture;
      delete nightCityLights1Texture;

      delete airplaneTexture;
      delete airplaneLightsRedTexture;
      delete airplaneLightsGreenTexture;
      delete airplaneLightsWhiteTexture;

      Mix_HaltMusic();
      Mix_FreeMusic(blues);
      Mix_CloseAudio();
    }

  private:
    SDL_Surface *screen;

    Texture *nowPlayingTexture;

    Texture *nightCityTexture;
    Texture *nightCityLights1Texture;

    Texture *airplaneTexture;
    Texture *airplaneLightsRedTexture;
    Texture *airplaneLightsGreenTexture;
    Texture *airplaneLightsWhiteTexture;

    NightCityEntity *nowPlaying;

    NightCityEntity *nightCity;
    NightCityEntity *nightCityLights1;

    AirplaneEntity *airplane;

    Mix_Music *blues;

    uint32 NextPowerOfTwo(uint32 n)
    {
      --n;
      n |= n>>1;
      n |= n>>2;
      n |= n>>4;
      n |= n>>8;
      n |= n>>16;
      ++n;

      return n;
    }

    Texture* MakeTexture(SDL_Surface *surface)
    {
      GlTexture texture;
      GLint colors;
      GLenum textureFormat;

      GLint width = surface->w;
      GLint height = surface->h;

      //
      GLint maxTextureSize;
      glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
      if(surface->w > maxTextureSize || surface->h > maxTextureSize)
        _d_log_warn("width > maxTextureSize || height > maxTextureSize");

      //
      if((width & (width - 1)))
        width = NextPowerOfTwo(width);

      if((height & (height - 1)))
        height = NextPowerOfTwo(height);

      //
      colors = surface->format->BytesPerPixel;
      if(colors == 4)
      {
        if(surface->format->Rmask == 0x000000ff)
          textureFormat = GL_RGBA;
        else
          textureFormat = GL_BGRA;
      }
      elif(colors == 3)
      {
        if(surface->format->Rmask == 0x000000ff)
          textureFormat = GL_RGB;
        else
          textureFormat = GL_BGR;
      }
      else
      {
        _d_log_warn(_d_file_line);
      }

      _d_log_info("Img: " << width << "*" << height << ", RGBA: " << (textureFormat == GL_RGBA));
 
      //
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
 
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST/*GL_LINEAR*/);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST/*GL_LINEAR*/);
 
      glTexImage2D(GL_TEXTURE_2D, 0, colors, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, null);
      glTexSubImage2D(GL_TEXTURE_2D, 0,
        0, 0, surface->w, surface->h,
        textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

      return new Texture(texture, surface->w, surface->h, (double)surface->w / (double)width, (double)surface->h / (double)height);
    }

    #if _d_os_win
      SDL_RWops* LoadResource(int resourceId)
      {
        HRSRC resRef = FindResourceA(null, MAKEINTRESOURCEA(resourceId), "FOO");
        if(!resRef)
          _d_log_fatal(_d_file_line << ", resourceId: " << resourceId);

        HGLOBAL resPtr = ::LoadResource(null, resRef);
        if(!resPtr)
          _d_log_fatal(_d_file_line << ", resourceId: " << resourceId);

        void *resData = LockResource(resPtr);
        if(!resData)
          _d_log_fatal(_d_file_line << ", resourceId: " << resourceId);

        SDL_RWops *rw = SDL_RWFromMem(resData, SizeofResource(null, resRef));

        UnlockResource(resRef);

        return rw;
      }
    #endif
};

//
//
//
void SignalHandlerFpe(int s)
{
  _d_log_fatal("Caught " << _d_funcname);
}

void SignalHandlerIll(int s)
{
  _d_log_fatal("Caught " << _d_funcname);
}

void SignalHandlerSeg(int s)
{
  _d_log_fatal("Caught " << _d_funcname);
}

int main(int argc, char **argv)
{
  //
  signal(SIGFPE, SignalHandlerFpe);
  signal(SIGILL, SignalHandlerIll);
  signal(SIGSEGV, SignalHandlerSeg);

  // Defaults.
  uint32 screenWidth = _d_app_default_screen_width;
  uint32 screenHeight = _d_app_default_screen_height;
  uint32 soundVolume = _d_app_default_sound_volume;
  char *windowCaption = _d_app_window_caption;

  //
  ;

  //
  App app(screenWidth, screenHeight, soundVolume, windowCaption);
  app.Init();
  app.Run();
  app.Destroy();

  return 0;
}
