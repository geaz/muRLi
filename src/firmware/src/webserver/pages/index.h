const char IndexPage[] PROGMEM = R"=====(
<html>
	<head>
			<title>muRLi - Home</title>
	</head>
<body>
	<form method='get' action='setSSID'>
        <input name='ssid' length=32>
        <input name='pass' length=64>
        <input type='submit'>
    </form>
</body>
</html>
)=====";