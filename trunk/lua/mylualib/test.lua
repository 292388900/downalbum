--[[TESTCODE]]

require "mylib.core"


a=1

t={}

f=function()  end

dir , path, name = mylib.getcurluapath()
mylib.msgbox(dir,path ,name)
mylib.msgbox(mylib.getcurluapath())

--dir = mylib.getcurluapath()
--mylib.msgbox(dir)

--
--mylib.msgbox( mylib.gethtml('www.baidu.com') )
--mylib.msgbox( mylib.gethtml('http://www.baidu.com') )

--s=mylib.gethtml('http://space.feixin.10086.cn/')
--mylib.msgbox( s )
--s=mylib.utf8togbk(s)
--mylib.msgbox( s )
--mylib.msgbox( 'a=' , a, 'b=' , b, t, f )

