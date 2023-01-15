#include "dio.h"

void dio_set_direction(dio_t pin, dio_direction_t direction)
{
	uint8_t port = (pin >> 8);
	pin = pin & 0xff;
	switch (port)
	{
	case DIO_PORTA:
		_assign_bit(DDRA, pin, direction);
		// DDRA |= (direction << pin);
		break;
	case DIO_PORTB:
		_assign_bit(DDRB, pin, direction);
		break;
	case DIO_PORTC:
		_assign_bit(DDRC, pin, direction);
		break;
	case DIO_PORTD:
		_assign_bit(DDRD, pin, direction);
		break;

	default:
		// TODO report error
		break;
	}
}

void dio_set_level(dio_t pin, dio_level_t value)
{
	uint8_t port = (pin >> 8);
	pin = pin & 0xff;
	switch (port)
	{
	case DIO_PORTA:
		_assign_bit(PORTA, pin, value);
		break;
	case DIO_PORTB:
		_assign_bit(PORTB, pin, value);
		break;
	case DIO_PORTC:
		_assign_bit(PORTC, pin, value);
		break;
	case DIO_PORTD:
		_assign_bit(PORTD, pin, value);
		break;

	default:
		// TODO report error
		break;
	}
}

dio_level_t dio_get_level(dio_t pin)
{
	uint8_t port = (pin >> 8);
	pin = pin & 0xff;
	switch (port)
	{
	case DIO_PORTA:
		return (PINA >> pin) & 1;
		break;
	case DIO_PORTB:
		return (PINB >> pin) & 1;
		break;
	case DIO_PORTC:
		return (PINC >> pin) & 1;
		break;
	case DIO_PORTD:
		return (PIND >> pin) & 1;
		break;

	default:
		// TODO report error
		break;
	}
}

void dio_set_port_direction(dio_port_t port, dio_direction_t direction)
{
	if (direction == DIO_DIRECTION_INPUT)
	{
		switch (port)
		{
		case DIO_PORTA:
			DDRA = 0;
			break;
		case DIO_PORTB:
			DDRB = 0;
			break;
		case DIO_PORTC:
			DDRC = 0;
			break;
		case DIO_PORTD:
			DDRD = 0;
			break;
		default:
			// TODO report error
			break;
		}
	}
	else if (direction == DIO_DIRECTION_OUTPUT)
	{
		switch (port)
		{
		case DIO_PORTA:
			DDRA = 0xff;
			break;
		case DIO_PORTB:
			DDRB = 0xff;
			break;
		case DIO_PORTC:
			DDRC = 0xff;
			break;
		case DIO_PORTD:
			DDRD = 0xff;
			break;
		default:
			// TODO report error
			break;
		}
	}
}

void dio_set_port_level(dio_port_t port, dio_level_t value)
{
	switch (port)
	{
	case DIO_PORTA:
		PORTA = value;
		break;
	case DIO_PORTB:
		PORTB = value;
		break;
	case DIO_PORTC:
		PORTC = value;
		break;
	case DIO_PORTD:
		PORTD = value;
		break;
	default:
		// TODO report error
		break;
	}
}

void dio_off()
{
	for (dio_port_t port = DIO_PORTA; port < DIO_PORTD; port++)
	{
		dio_set_port_level(port, DIO_LEVEL_OFF);
		dio_set_port_direction(port, DIO_DIRECTION_OFF);
	}
}