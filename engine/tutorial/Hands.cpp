let handSize = 0.25 * ui.instructionConfiguration.scale;
let handCircleR = handSize;
let handStartAngle = 30;
let handEndAngle = 60;
let handUpperLength = 0.5;
let handHorizontalLength = 0.5;

SonolusApi drawFrozenHand(let hands, let x, let y, var percent, bool in = true, bool out = true) {
	FUNCBEGIN
	let angle = If(
		percent < 0.25, 
		handStartAngle,
		If(
			percent < 0.75,
			(percent - 0.25) / 0.5 * (handEndAngle - handStartAngle) + handStartAngle,
			handEndAngle
		)
	);
	let goalX = x, goalY = y;
	let centerX = goalX + handCircleR * Sin(handEndAngle / 180 * acos(-1));
	let centerY = goalY - handCircleR * Cos(handEndAngle / 180 * acos(-1));
	let currentX = centerX - handCircleR * Sin(angle / 180 * acos(-1));
	let currentY = centerY + handCircleR * Cos(angle / 180 * acos(-1));
	let a = If(
		percent < 0.25,
		(!in) ? 1 : 1.0 * percent / 0.25,
		If(
			percent < 0.75,
			1,
			(!out) ? 1 : 1.0 - 1.0 * (percent - 0.75) / 0.25
		)
	);
	Paint(hands, currentX, currentY, handSize, angle, 1919810, a);
	return VOID;
}

SonolusApi drawUpperHand(let hands, let x, let y, let percent, let angle, bool in = false, bool out = true) {
	FUNCBEGIN
	let goalX = x;
	let goalY = y;
	let currentX = goalX;
	let currentY = goalY + If(
		percent < 0.25,
		0,
		If(
			percent < 0.75,
			(percent - 0.25) / 0.5 * handUpperLength,
			handUpperLength
		)
	);
	let a = If(
		percent < 0.25,
		(!in) ? 1 : 1.0 * percent / 0.25,
		If(
			percent < 0.75,
			1,
			(!out) ? 1 : 1.0 - 1.0 * (percent - 0.75) / 0.25
		)
	);
	Paint(hands, currentX, currentY, handSize, angle, 2000, a);
	return VOID;
}

SonolusApi drawHorizontalHand(let hands, let x, let y, let percent, let angle, bool in = false, bool out = true) {
	FUNCBEGIN
	let goalX = x;
	let goalY = y;
	let currentX = goalX + If(
		percent < 0.25,
		0,
		If(
			percent < 0.75,
			(percent - 0.25) / 0.5 * handHorizontalLength,
			handHorizontalLength
		)
	);
	let currentY = goalY;
	let a = If(
		percent < 0.25,
		(!in) ? 1 : 1.0 * percent / 0.25,
		If(
			percent < 0.75,
			1,
			(!out) ? 1 : 1.0 - 1.0 * (percent - 0.75) / 0.25
		)
	);
	Paint(hands, currentX, currentY, handSize, angle, 2000, a);
	return VOID;
}