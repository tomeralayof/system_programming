#ifndef __PINGPONG_H__
#define __PINGPONG_H__

typedef enum status
{
    FAILURE = -1,
    SUCCESS = 0
}status_t;

status_t PingPong(void);
status_t Ping(void);
status_t PingEx4(void);

#endif /* __PINGPONG_H__ */