/**
  *@ FileName: BspState.h
  *@ Author: CzrTuringB
  *@ Brief:
  *@ Time: Sep 22, 2024
  *@ Requirement：
  */
#ifndef __BSPSTATE_H
#define __BSPSTATE_H
/* Includes ------------------------------------------------------------------*/
/* Data ------------------------------------------------------------------*/
typedef enum
{
	BspError = 0,
	BspOk	 = 1,
}BspState;
typedef enum
{
	Enable = 0,
	Disable = 1,
}EnDis;
typedef enum
{
	False = 0,
	True = 1,
}Bool;
#endif
