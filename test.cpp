#include <iostream>
#include <map>
#include <vector>
#include "StateList.hpp"

class State
{
private:
    /* data */
public:
    StateList state_var;
    std::vector<StateList> next_state_list;

    // Initialize with a starting state and a vector of output states 
    State(StateList input_state = StateList::NOT_SET_UP, std::vector<StateList> output_states = {}): 
        state_var{ input_state }, next_state_list{ output_states }
    {
        std::cout << static_cast<int>(this->state_var) << std::endl;
        for(size_t i = 0; i < this->next_state_list.size(); i++)
        {
            std::cout << static_cast<int>(this->next_state_list.at(i)) << std::endl;
        }
    }

    virtual void action()
    {
        return;
    }

    virtual StateList transitionCheck()
    {
        std::cout << "You can't have a state that doesn't transition anywhere! My state variable is "<< static_cast<int>(this->state_var) << std::endl;
        exit(EXIT_FAILURE);
    }

};


class stateEngine
{
private:
    /* data */
public:
    State *current_state;

    stateEngine(State *someState = new State):
        current_state{ someState }
    {
    };

    template <class T>
    void changeClass()
    {
        delete current_state;
        current_state = new T;
        return;
    }

    virtual void update()
    {
        current_state->transitionCheck();
    }

    virtual void end()
    {
        exit(EXIT_SUCCESS);
    }
};


class idleState : public State
{
private:
    /* data */
public:
    idleState(StateList this_state = StateList::IDLE, std::vector<StateList> next_states = { StateList::TEST }):
        State(this_state, next_states)
        {
        }

    virtual void action()
    {
        std::cout << "I'm in idle!" << std::endl;
    }

    virtual StateList transitionCheck()
    {
        // Range-based for loop is easiest to use here
        for(auto const &i : next_state_list)
        {
            if(i == StateList::TEST)
            {
                std::cout << "Going to test!" << std::endl;
                return i;
            }
        }

        return state_var;
    }
};


class testState : public State
{
private:
    /* data */
public:
    testState(StateList this_state = StateList::TEST, std::vector<StateList> next_states = { StateList::RELAY }):
        State(this_state, next_states)
    {
    }

    virtual void action()
    {
        std::cout << "I'm setting up the test bed here!" << std::endl;
    }

    virtual StateList transitionCheck()
    {
        // Range-based for loop is easiest to use here
        for(auto const &i : next_state_list)
        {
            if(i == StateList::RELAY)
            {
                std::cout << "Going to relay!" << std::endl;
                return i;
            }
        }

        return state_var;
    }
};


class relayState : public State
{
private:
    /* data */
public:
    // Data can start here
    bool relay1_out = false;
    uint8_t relay1_count = 0;

    relayState(StateList this_state = StateList::RELAY, std::vector<StateList> next_states = { StateList::RESET }):
        State(this_state, next_states)
        {
        }

    virtual void action()
    {
        if(relay1_out == true)
        {
            std::cout << "toggling to false" << std::endl;
        }
        else
        {
            std::cout << "toggling to true" << std::endl;
        }

        relay1_out = !relay1_out;
        relay1_count++;
    }

    virtual StateList transitionCheck()
    {
        // Range-based for loop is easiest to use here
        for(auto const &i : next_state_list)
        {
            if(i == StateList::RESET && relay1_count == 10)
            {
                std::cout << "Going to reset!" << std::endl;
                return i;
            }
        }

        return state_var;
    }
};


class resetState : public State
{
private:
    /* data */
public:
    resetState(StateList this_state = StateList::RESET, std::vector<StateList> next_states = { StateList::IDLE }):
        State(this_state, next_states)
        {
        }

    virtual void action()
    {
        std::cout << "I'm in idle!" << std::endl;
    }

    virtual StateList transitionCheck()
    {
        // Range-based for loop is easiest to use here
        for(auto const &i : next_state_list)
        {
            if(i == StateList::TEST)
            {
                std::cout << "Going to test!" << std::endl;
                return i;
            }
        }

        return state_var;
    }
};


class testBed : public stateEngine
{
private:
    /* data */
public:
    // I think it's fair to put the enum-to-class map here
    const std::map<StateList, void(*)> StateChanges = 
    {
        { StateList::IDLE,  changeClass<idleState>  },
        { StateList::TEST,  changeClass<testState>  },
        { StateList::RELAY, changeClass<relayState> },
        { StateList::RESET, changeClass<resetState> }
    };

    testBed()
    {
        stateEngine(idleState());
    }

    ~testBed()
    {
    }
    
    virtual void update()
    {
        // transitionCheck returns a key to a dictionary we have to make
        // this dictionary uses StateList enums for keys and class constructors for data
        StateList update_var = current_state->transitionCheck();
        if(update_var != current_state->state_var)
        {
            // Maybe have a function pointer here so you can retrieve the appropriate
            //   changeClass<type>() from some map.
            void (*changeClassType)() = { NULL };
            
            // Cry here if you're doing embedded. Using 'at()' with a bad key returns an Out of Range exception.
            try
            {
                changeClassType = StateChanges.at(update_var);
            }
            catch(const std::out_of_range& e)
            {
                std::cerr << "In an unknown state: " << e.what() << std::endl << "Failing on exit." << std::endl;
                exit(EXIT_FAILURE);
            }

            changeClassType();
            return;
        }
    }

    virtual void end()
    {
        std::cout << "Exiting testbed" << std::endl;
        exit(EXIT_SUCCESS);
    }
};


int main()
{
    testBed test();

    while(true)
    {
        test.update();
    }

    return 0;
}