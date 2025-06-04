FuncNode handSize = 0.25 * configuration.instruction.scale;
FuncNode handCircleR = handSize;
FuncNode handStartAngle = 30;
FuncNode handEndAngle = 60;
FuncNode handUpperLength = 0.5;

SonolusApi drawFrozenHand(var hands, var x, var y, var percent, bool in = true, bool out = true) {
	var angle = If(
		percent < 0.25, 
		handStartAngle,
		If(
			percent < 0.75,
			(percent - 0.25) / 0.5 * (handEndAngle - handStartAngle) + handStartAngle,
			handEndAngle
		)
	);
	var goalX = x, goalY = y;
	var centerX = goalX + handCircleR * Sin(handEndAngle / 180 * acos(-1));
	var centerY = goalY - handCircleR * Cos(handEndAngle / 180 * acos(-1));
	var currentX = centerX - handCircleR * Sin(angle / 180 * acos(-1));
	var currentY = centerY + handCircleR * Cos(angle / 180 * acos(-1));
	var a = If(
		percent < 0.25,
		(!in) ? 1 : 1.0 * percent / 0.25,
		If(
			percent < 0.75,
			1,
			(!out) ? 1 : 1.0 - 1.0 * (percent - 0.75) / 0.25
		)
	);
	Paint(hands, currentX, currentY, handSize, angle, 1919810, a);
}

SonolusApi drawUpperHand(var hands, var x, var y, var percent, var angle, bool in = false, bool out = true) {
	var goalX = x;
	var goalY = y;
	var currentX = goalX;
	var currentY = goalY + If(
		percent < 0.25,
		0,
		If(
			percent < 0.75,
			(percent - 0.25) / 0.5 * handUpperLength,
			handUpperLength
		)
	);
	var a = If(
		percent < 0.25,
		(!in) ? 1 : 1.0 * percent / 0.25,
		If(
			percent < 0.75,
			1,
			(!out) ? 1 : 1.0 - 1.0 * (percent - 0.75) / 0.25
		)
	);
	Paint(hands, currentX, currentY, handSize, angle, 1919810, a);
}

SonolusApi drawHorizontalHand(var hands, var x, var y, var dx, var percent, var angle, bool in = false, bool out = true) {
	var goalX = x;
	var goalY = y;
	var currentX = goalX + If(
		percent < 0.25,
		0,
		If(
			percent < 0.75,
			(percent - 0.25) / 0.5 * dx,
			dx
		)
	);
	var currentY = goalY;
	var a = If(
		percent < 0.25,
		(!in) ? 1 : 1.0 * percent / 0.25,
		If(
			percent < 0.75,
			1,
			(!out) ? 1 : 1.0 - 1.0 * (percent - 0.75) / 0.25
		)
	);
	Paint(hands, currentX, currentY, handSize, angle, 1919810, a);
}