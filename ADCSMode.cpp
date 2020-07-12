/*
 * ADCSMode.cpp
 *
 *  Created on: June 14th, 2020
 *      Author: Johan Monster
 *
 *  About this implementation:
 *  At the time of writing, several things relating to the ADCS have not been defined, namely:
 *   - The contents of the ADCS software are unknown at the time of writing.
 *   - No bus telecommunication frame has yet been defined for the ADCS.
 *   - The current ADCS housekeeping container contains neither attitude sensor input, nor output from higher-level
 *      functions in the ADCS software that can be used to determine the attitude state of the satellite.
 *
 *  As a result, to still write a meaningful OBC mode, a number of assumptions have been made about how the ADCS might
 *   be implemented:
 *    1. The ADCS outputs a generic RotationSpeed parameter, which is enough to judge whether detumbling is needed.
 *    2. The threshold for when detumbling is and isn't needed can be fetched from variableContainer.getRotationSpeedLimit()
 *    3. The actual detumbling is handled by the ADCS subsystem, which can be activated through a generic function (to be implemented later)
 *
 */

// TODO:
// [ ] Test 
// [ ] Finalize documentation


#define ADCSMODE_DEBUG

#include "ADCSMode.h"

extern OBCVariableContainer variableContainer;


// Definition of ADCSMode()
void ADCSMode(const long& upTime) {
    #ifdef ADCSMODE_DEBUG
        Console::log("[DEBUG] ADCSMode() activated.\n");
    #endif
    // === ADCSM-OBC-1 ===

    // Proceed with initialization of ADCS power line if the flag is IDLE
    if( variableContainer.getADCSState() == IDLE) {
        #ifdef ADCSMODE_DEBUG
            Console::log("[Debug] ADCSMode() : ADCSState = IDLE.\n");
        #endif

        // Initialize ADCS power line (ADCSM-OBC-2), returns 1 if ADCS passes initialization.
        bool initialization = ADCS_init(upTime);

        // Do work for as long as rotation rates and power levels are high enough.
        if (initialization == 1) {
            bool work = ADCS_work(upTime);
        }

        if (initialization == 0) {
            variableContainer.setMode(SAFEMODE);
            variableContainer.setADCSState(DISABLED);
        }
    }

    else if(variableContainer.getADCSState() == DETUMBLE){
        
        // <Do what exactly?>

        #ifdef ADCSMODE_DEBUG
            Console::log("[Debug] ADCSMode() : ADCSState = DETUMBLE.\n");
        #endif
    }

    else if(variableContainer.getADCSState() == DISABLED){
        // If the ADCS is disabled, don't perform work and switch to nominal mode.
        // Check: Can the OBC get stuck in a NOMINAL->ADCSMODE->NOMINAL loop here?
        #ifdef ADCSMODE_DEBUG
            Console::log("[Debug] ADCSMode() : ADCSState = DISABLED. \n");
        #endif
            variableContainer.setMode(NOMINALMODE);
    }
}


// === ADCSM-OBC-2 ===
// Initialize ADCS power line
bool ADCS_init(const long& upTime) {

    // TODO Dummy value, remove later when Health Checks are implemented
    ADCSHealthResult HealthResult;

    bool success;
    // Count how many times ADCS has been power cycled
    unsigned int strikes = 0; 
    unsigned int allowed_strikes = 5;

    // Define a desired initialization time in [s]
    unsigned long ADCS_init_timer = upTime + 30;
    // Preallocate a cycling timer
    unsigned long cycling_timer;


    // Activate power line V2
    variableContainer.setADCSPowerState(UNINITIALIZED);
    bool V2on = LineControl(1,1,0,0);
    
    #ifdef ADCSMODE_DEBUG
        Console::log("[Debug] ADCS_init(): Attempting to initialize ADCSPowerState. \n");
    #endif

    // Start initialization loop.
    // For more information, check the ADCSM-OBC-2 flow chart.
    while(true) {

        // Init timer NOT done && powerstate = uninitialized
        if ((upTime <= ADCS_init_timer) && (variableContainer.getADCSPowerState() == UNINITIALIZED)){
            // Do nothing
        }


        // Init timer done && powerstate = uninitialized
        if ((upTime > ADCS_init_timer) && (variableContainer.getADCSPowerState() == UNINITIALIZED)){
            variableContainer.setADCSPowerState(INITIALIZED);

            // TODO perform health check

            // DUMMY value -> TODO REPLACE this with an ADCSHealth value fetched from the variableContainer once implemented:
            ADCSHealthResult HealthResult = ADCS_GOOD;
        }

        
        // Cycling timer NOT done && powerstate = cycling && max strikes NOT reached
        if ((upTime <= cycling_timer) && (variableContainer.getADCSPowerState() == CYCLING) && (strikes != allowed_strikes)){
            // Do nothing
        }


        // Cycling timer done && powerstate = cycling && max strikes NOT reached
        if ((upTime > cycling_timer) && (variableContainer.getADCSPowerState() == CYCLING) && (strikes != allowed_strikes)){

            // Reactivate power line V2
            V2on = LineControl(1,1,0,0);

            // Set power state to uninitialized
            variableContainer.setADCSPowerState(UNINITIALIZED);

            // Reset initialization timer
            ADCS_init_timer = upTime + 30;
        }

        
        // Max strikes reached && powerstate = cycling
        if ((variableContainer.getADCSPowerState() >= CYCLING) && (strikes == allowed_strikes)){
            // Deactivate power line V2
            variableContainer.setADCSPowerState(CYCLING);

            // Set failure condition and break
            success = 0;
            break;
        }

        
        // Health check NOT okay && powerstate = initialized
        if ((HealthResult == ADCS_BAD) && (variableContainer.getADCSPowerState() == INITIALIZED)){
            // Generate a cycling timer
            cycling_timer = upTime + 3;

            // Turn the V2 power line off
            V2on = LineControl(1,0,0,0);

            // Indicate that the ADCS power line is cycling
            variableContainer.setADCSPowerState(CYCLING);

            // Add a strike, indicating that a power cycle was initiated.
            strikes++;
        }


        // Health check okay && powerstate = initialized
        if ((HealthResult == ADCS_GOOD) && (variableContainer.getADCSPowerState() == INITIALIZED)){
            success = 1;
            break;
        }

    }

    return success;
}


// === ADCSM-OBC-3 ===
// Periodically monitor rotational speeds and operate ADCS based on rates. 
bool ADCS_work(const long& upTime) {
    bool success;
    unsigned long detumble_timer;

    if(variableContainer.getADCSState() == IDLE) {

        if(variableContainer.getRotateSpeed() >= variableContainer.getRotateSpeedLimit()) {
            
            // TODO <Tell ADCS to start powering torquers>

            #ifdef ADCSMODE_DEBUG
                Console::log("[Debug] ADCS_work(): Zoom...zoom...zoom... magnetotorquers activated. \n");
            #endif

            variableContainer.setADCSState(DETUMBLE);

            // Define detumble timer in [s]
            detumble_timer = upTime + 180;
        }
        else if(variableContainer.getRotateSpeed() < variableContainer.getRotateSpeedLimit()) {
            success = 1;
			variableContainer.setADCSState(IDLE);
            variableContainer.setMode(NOMINALMODE);
        }
    }

    else if((variableContainer.getADCSState() == DETUMBLE) & (upTime < detumble_timer)) {
        if(variableContainer.getRotateSpeed() >= variableContainer.getRotateSpeedLimit()) {
            
            // TODO <Tell ADCS to start powering torquers>

            #ifdef ADCSMODE_DEBUG
                Console::log("[Debug] ADCS_work(): Zoom...zoom...zoom... magnetotorquers activated. \n");
            #endif
        }

        else if(variableContainer.getRotateSpeed() < variableContainer.getRotateSpeedLimit()) {

            // TODO <Somehow tell ADCS to stop powering torquers>
			
			success = 1;
            variableContainer.setADCSState(IDLE);
            variableContainer.setMode(NOMINALMODE);
        }
    }
    else if((variableContainer.getADCSState() == DETUMBLE) & (upTime >= detumble_timer)) {
            
        // TODO <Somehow tell ADCS to stop powering torquers>
		
		success = 0;
        variableContainer.setADCSState(DISABLED);
        variableContainer.setMode(SAFEMODE);
    }

    return success;
}


