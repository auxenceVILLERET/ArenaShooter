#ifndef TEST_STATE_MACHINE_HPP_INCLUDED
#define TEST_STATE_MACHINE_HPP_INCLUDED

#include <StateMachine/StateMachine.h>
#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( StateMachine )

    StateMachine sm;

    // Add Idle state
    String idle = "Idle";
    sm.AddAction(idle, OnBeginIdle, OnUpdateIdle, OnEndIdle);

    // Add Running state
    String running = "Running";
    sm.AddAction(running, OnBeginRunning, OnUpdateRunning, OnEndRunning);

    // Add Transition: Idle ? Running when start is requested
    StateMachine::Condition condition { IsStartRequested };
    Vector<StateMachine::Condition> conditions { condition };
    sm.AddTransition(conditions, running);

    // Add Transition: Running ? Idle when the run is done
    StateMachine::Condition condition2 { IsDoneRunning };
    conditions.clear();
    conditions.push_back(condition2);
    sm.AddTransition(conditions, idle);

    // === Simulate Updates ===
    Console::Log("=== Initial Update ===");
    sm.Update();  // Should be Idle

    Console::Log("=== Request Start ===");
    s_isStartRequested = true;  // Set the condition to true

    Console::Log("=== Update Triggering Transition ===");
    sm.Update();  // Should transition to Running
    s_isStartRequested = false;

    Console::Log("=== Update in Running State ===");
    sm.Update();  // Should call OnUpdateRunning

    Console::Log("=== Finilazing Running State ===");
    s_isDoneRunning = true; // Set the condition to true

    Console::Log("=== Update Triggering Transition ===");
    sm.Update();  // Should transition back to Idle

END_TEST_MAIN_FUNCTION( StateMachine )

inline static bool s_isStartRequested = false;
inline static bool s_isDoneRunning = false;

// === Idle State Functions ===
static void OnBeginIdle() { Console::Log("[Idle] Begin"); }
static void OnUpdateIdle() { Console::Log("[Idle] Updating... Waiting for start request."); }
static void OnEndIdle() { Console::Log("[Idle] End"); }

// === Running State Functions ===
static void OnBeginRunning() { Console::Log("[Running] Begin"); }
static void OnUpdateRunning() { Console::Log("[Running] Updating... Doing work."); }
static void OnEndRunning() { Console::Log("[Running] End"); }

// === Condition Function ===
static bool IsStartRequested() { return s_isStartRequested; }
static bool IsDoneRunning() { return s_isDoneRunning; }

END_TEST( StateMachine )


#endif