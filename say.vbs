'say.vbs
set s = CreateObject("SAPI.SpVoice")
s.Speak Wscript.Arguments(0), 10
s.WaitUntilDone(1000)