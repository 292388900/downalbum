
functions = {
{'','print','print(all data type)'},
{'','msgbox','msgbox(all data type)'},
};

local space = nil;
local name = nil;
local desc = nil;

for i = 1,#functions do
	space = functions[i][1];
	name = functions[i][2];
	desc = functions[i][3];
	if name~=nil then
		if space==nil then
			space='';
		end
		if desc==nil then
			desc='';
		end
	LS_InsertFunction(space,name,desc);
	end
end
