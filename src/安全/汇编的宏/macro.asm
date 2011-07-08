.386
  .model flat,stdcall
  option casemap:none
  
  include windows.inc
  include user32.inc
  include kernel32.inc
  include masm32.inc
  include D:\RadASM\masm32\macros\macros.asm	;里面的宏好强大啊
  includelib masm32.lib
  includelib kernel32.lib
  includelib user32.lib
  
  .code
start:  
  mov  eax,100
  invoke  MessageBox,NULL,str$(eax), chr$("hello"), MB_OK
  exit
  end  start
