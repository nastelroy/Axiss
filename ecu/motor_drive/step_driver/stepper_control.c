/* 
 * didapat dari didapat dari http://reprap.org/bin/view/Main/Arduino_GCode_Interpreter
 * 
 * diubah seperlunya untuk CNC Fana
 * 
 * furkan jadid
 * 17 Maret 2010
 * daun biru engineering
 * 
 */

#define dprintf		printf

#define X_STEP_PIN	1
#define Y_STEP_PIN	2
#define Z_STEP_PIN	3

#define X_MIN_PIN	1
#define Y_MIN_PIN	1
#define Z_MIN_PIN	1

#define X_MAX_PIN	1
#define Y_MAX_PIN	1
#define Z_MAX_PIN	1

extern xSemaphoreHandle step_sem;

long max(long A, long B)
{
	if ( A > B ) return A;
	else return B;
}

//init our variables
long max_delta;
long x_counter;
long y_counter;
long z_counter;
bool x_can_step;
bool y_can_step;
bool z_can_step;
int milli_delay;

struct t_step *pstep;

void init_steppers()
{
	dprintf("%s():\r\n", __FUNCTION__);
	
	//turn them off to start.
	disable_steppers();
	
	//init our points.
	current_units.x = 0.0;
	current_units.y = 0.0;
	current_units.z = 0.0;
	target_units.x = 0.0;
	target_units.y = 0.0;
	target_units.z = 0.0;
	
	/*
	pinMode(X_STEP_PIN, OUTPUT);
	pinMode(X_DIR_PIN, OUTPUT);
	pinMode(X_ENABLE_PIN, OUTPUT);
	pinMode(X_MIN_PIN, INPUT);
	pinMode(X_MAX_PIN, INPUT);
	
	pinMode(Y_STEP_PIN, OUTPUT);
	pinMode(Y_DIR_PIN, OUTPUT);
	pinMode(Y_ENABLE_PIN, OUTPUT);
	pinMode(Y_MIN_PIN, INPUT);
	pinMode(Y_MAX_PIN, INPUT);
	
	pinMode(Z_STEP_PIN, OUTPUT);
	pinMode(Z_DIR_PIN, OUTPUT);
	pinMode(Z_ENABLE_PIN, OUTPUT);
	pinMode(Z_MIN_PIN, INPUT);
	pinMode(Z_MAX_PIN, INPUT);
	*/
	
	//figure our stuff.
	calculate_deltas();
}

void dda_move(long micro_delay)
{
	dprintf("%s(): delay %d\r\n", __FUNCTION__, micro_delay);
	
	
	/*
	//enable our steppers
	digitalWrite(X_ENABLE_PIN, HIGH);
	digitalWrite(Y_ENABLE_PIN, HIGH);
	digitalWrite(Z_ENABLE_PIN, HIGH);
	*/
	//figure out our deltas
	max_delta = max(delta_steps.x, delta_steps.y);
	max_delta = max(delta_steps.z, max_delta);

	//init stuff.
	long x_counter = -max_delta/2;
	long y_counter = -max_delta/2;
	long z_counter = -max_delta/2;
	
	//our step flags
	bool x_can_step = 1;		//0
	bool y_can_step = 1;
	bool z_can_step = 1;
	
	
	xSemaphoreGive( step_sem );	
	
	#if 0
	if (micro_delay >= 16383)
		milli_delay = micro_delay / 1000;
	else
		milli_delay = 0;

	//do our DDA line!
	do
	{
		x_can_step = can_step(X_MIN_PIN, X_MAX_PIN, current_steps.x, target_steps.x, x_direction);
		y_can_step = can_step(Y_MIN_PIN, Y_MAX_PIN, current_steps.y, target_steps.y, y_direction);
		z_can_step = can_step(Z_MIN_PIN, Z_MAX_PIN, current_steps.z, target_steps.z, z_direction);

		if (x_can_step)
		{
			x_counter += delta_steps.x;
			
			if (x_counter > 0)
			{
				do_step(X_STEP_PIN);
				x_counter -= max_delta;
				
				if (x_direction)
					current_steps.x++;
				else
					current_steps.x--;
			}
		}

		if (y_can_step)
		{
			y_counter += delta_steps.y;
			
			if (y_counter > 0)
			{
				do_step(Y_STEP_PIN);
				y_counter -= max_delta;

				if (y_direction)
					current_steps.y++;
				else
					current_steps.y--;
			}
		}
		
		if (z_can_step)
		{
			z_counter += delta_steps.z;
			
			if (z_counter > 0)
			{
				do_step(Z_STEP_PIN);
				z_counter -= max_delta;
				
				if (z_direction)
					current_steps.z++;
				else
					current_steps.z--;
			}
		}
		
		//extruder_manage_temperature();
				
		//wait for next step.
		/*
		if (milli_delay > 0)
			delay(milli_delay);			
		else
			delayMicroseconds(micro_delay);
			*/
	}
	while (x_can_step || y_can_step || z_can_step);
	
	//set our points to be the same
	current_units.x = target_units.x;
	current_units.y = target_units.y;
	current_units.z = target_units.z;
	calculate_deltas();
	#endif
	
}

bool can_step(byte min_pin, byte max_pin, long current, long target, byte direction)
{
	dprintf("%s():\r\n", __FUNCTION__);
	
	//stop us if we're on target
	if (target == current)
		return false;
	//stop us if we're at home and still going 
	else if (read_switch(min_pin) && !direction)
		return false;
	//stop us if we're at max and still going
	else if (read_switch(max_pin) && direction)
		return false;

	//default to being able to step
	return true;
}

void do_step(byte step_pin)
{
	dprintf("%s():\r\n", __FUNCTION__);
	
	/*
	digitalWrite(step_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(step_pin, LOW);
	*/
}

bool read_switch(byte pin)
{
	dprintf("%s():\r\n", __FUNCTION__);
	//dual read as crude debounce
	/*
	if ( SENSORS_INVERTING )
		return !digitalRead(pin) && !digitalRead(pin);
	else
		return digitalRead(pin) && digitalRead(pin);
		*/
}

long to_steps(float steps_per_unit, float units)
{
	dprintf("%s(): %f, unit %f\r\n", __FUNCTION__, steps_per_unit, units);
	
	return steps_per_unit * units;
}

void set_target(float x, float y, float z)
{
	dprintf("%s(): %f, %f, %f\r\n", __FUNCTION__, x, y, z);
	
	target_units.x = x;
	target_units.y = y;
	target_units.z = z;
	
	calculate_deltas();
}

void set_position(float x, float y, float z)
{
	dprintf("%s(): %f, %f, %f\r\n", __FUNCTION__, x, y, z);
	
	current_units.x = x;
	current_units.y = y;
	current_units.z = z;
	
	calculate_deltas();
}

void calculate_deltas()
{
	dprintf("%s():\r\n", __FUNCTION__);
	
	//figure our deltas.
	delta_units.x = abs(target_units.x - current_units.x);
	delta_units.y = abs(target_units.y - current_units.y);
	delta_units.z = abs(target_units.z - current_units.z);
				
	//set our steps current, target, and delta
	current_steps.x = to_steps(x_units, current_units.x);
	current_steps.y = to_steps(y_units, current_units.y);
	current_steps.z = to_steps(z_units, current_units.z);

	target_steps.x = to_steps(x_units, target_units.x);
	target_steps.y = to_steps(y_units, target_units.y);
	target_steps.z = to_steps(z_units, target_units.z);

	delta_steps.x = abs(target_steps.x - current_steps.x);
	delta_steps.y = abs(target_steps.y - current_steps.y);
	delta_steps.z = abs(target_steps.z - current_steps.z);
	
	//what is our direction
	x_direction = (target_units.x >= current_units.x);
	y_direction = (target_units.y >= current_units.y);
	z_direction = (target_units.z >= current_units.z);

	dprintf(" delta un %f %f %f\r\n", delta_units.x, delta_units.y, delta_steps.z);
	dprintf(" cur step %f %f %f\r\n", current_steps.x, current_steps.y, current_steps.z);
	dprintf(" tgt step %f %f %f\r\n", target_steps.x, target_steps.y, target_steps.z);
	dprintf(" delta step %f %f %f\r\n", delta_steps.x, delta_steps.y, delta_steps.z);
	dprintf(" direction %d, %d, %d\r\n", x_direction, y_direction, z_direction);
	//set our direction pins as well
	/*
	digitalWrite(X_DIR_PIN, x_direction);
	digitalWrite(Y_DIR_PIN, y_direction);
	digitalWrite(Z_DIR_PIN, z_direction);
	*/
}


long calculate_feedrate_delay(float feedrate)
{
	dprintf("%s():\r\n", __FUNCTION__);
	
	//how long is our line length?
	float distance = sqrt(delta_units.x*delta_units.x + delta_units.y*delta_units.y + delta_units.z*delta_units.z);
	long master_steps = 0;
	
	//find the dominant axis.
	if (delta_steps.x > delta_steps.y)
	{
		if (delta_steps.z > delta_steps.x)
			master_steps = delta_steps.z;
		else
			master_steps = delta_steps.x;
	}
	else
	{
		if (delta_steps.z > delta_steps.y)
			master_steps = delta_steps.z;
		else
			master_steps = delta_steps.y;
	}

	//calculate delay between steps in microseconds.  this is sort of tricky, but not too bad.
	//the formula has been condensed to save space.  here it is in english:
	// distance / feedrate * 60000000.0 = move duration in microseconds
	// move duration / master_steps = time between steps for master axis.

	return ((distance * 60000000.0) / feedrate) / master_steps;	
}

long getMaxSpeed()
{
	dprintf("%s():\r\n", __FUNCTION__);
	
	if (delta_steps.z > 0)
		return calculate_feedrate_delay(FAST_Z_FEEDRATE);
	else
		return calculate_feedrate_delay(FAST_XY_FEEDRATE);
}

void disable_steppers()
{
	dprintf("%s():\r\n", __FUNCTION__);
	
	/*
	//enable our steppers
	digitalWrite(X_ENABLE_PIN, LOW);
	digitalWrite(Y_ENABLE_PIN, LOW);
	digitalWrite(Z_ENABLE_PIN, LOW);
	*/
}
