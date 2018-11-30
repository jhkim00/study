import subprocess

def __makeExtra(extra) :	
	str = ""
	if (extra != None) :
		for (key, value) in extra.items() :
			str += " --es " + key + " " + value
	return str

def broadcast(actionName, extra=None) :
	cmd = "adb shell am broadcast -a " + actionName + __makeExtra(extra)
	print(cmd)
	subprocess.call(cmd)

#broadcast("com.test.jhkim.avnstudy.action.KEY_EVT", {"type" : "RADIO"})