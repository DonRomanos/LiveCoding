#include "logger.hpp"
#include <any>
#include <boost/hana.hpp>
#include <boost/sml.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
/*
References:
https://boost-ext.github.io/sml/overview.html
*/

namespace sml = boost::sml;

namespace states {
struct Initialization { };
struct WaitingForInput { };
struct ProcessingInput { };
struct HandleInputError { };
}

namespace events {
struct ReceiveInput {
    std::string input;
};
struct Success { };
struct Failure { };
}

struct IInputProcessor {
    virtual ~IInputProcessor() = default;

    [[nodiscard]] virtual bool processInput(std::string_view input) const = 0;
};

struct InputProcessor : public IInputProcessor {
    [[nodiscard]] bool processInput(std::string_view input) const override
    {
        std::cout << "Processing input: " << input << std::endl;
        if (input == "Success") {
            return true;
        }
        return false;
    }
};

using DispatchFunction = std::function<bool(const std::any& Event)>;

struct IDispatcher {
    IDispatcher(DispatchFunction function)
        : m_function(std::move(function))
    {
    }

    template <typename T>
    bool processEvent(const T& event)
    {
        return m_function(event);
    }

private:
    DispatchFunction m_function;
};

namespace actions {
constexpr auto processInput = [](const events::ReceiveInput& event, IInputProcessor& processor, IDispatcher& dispatcher) {
    if (processor.processInput(event.input)) {
        dispatcher.processEvent(events::Success {});
    } else {
        dispatcher.processEvent(events::Failure {});
    }
};

constexpr auto printEventType = [](const auto& event) {
    std::cout << "Event type: " << sml::aux::get_type_name<decltype(event)>() << std::endl;
};
}

struct ExampleStateMachine {
    auto operator()() const
    {
        namespace sml = boost::sml;

        using namespace states;
        using namespace events;

        // clang-format off
        return sml::make_transition_table(
/*
Note: For using context-based states in transition tables please refer to src/common/fsm/SmlExtensions.h

State                                     Event                                               Action                                      Target
+-----------------------------------------+---------------------------------------------------+-------------------------------------------+--------------------------------------------------+*/
*sml::state<Initialization>                                                                                                                = sml::state<WaitingForInput>

,sml::state<WaitingForInput>              + sml::event<ReceiveInput>                           / actions::printEventType                   = sml::state<ProcessingInput>

,sml::state<ProcessingInput>              + sml::on_entry<ReceiveInput>                        / actions::processInput
,sml::state<ProcessingInput>              + sml::event<Success>                                                                            = sml::state<WaitingForInput>
,sml::state<ProcessingInput>              + sml::event<Failure>                                                                            = sml::X
);
        // clang-format on
    }
};

template <typename... Events>
constexpr auto toHanaTuple(const boost::sml::aux::type_list<Events...>&)
{
    return boost::hana::tuple_t<Events...>;
}

template <typename Target>
struct ActualDispatcher : public IDispatcher {
    using EventTypes = typename Target::events;

    ActualDispatcher()
        : IDispatcher([this](const std::any& event) {
            return this->processEvent(event);
        })
    {
    }

    void setTarget(Target& target) { m_target = &target; }

    bool processEvent(const std::any& event)
    {
        auto wasDispatched = boost::hana::any_of(
            toHanaTuple(EventTypes {}),
            [&event, this](auto eventType) {
                using TEvent = typename decltype(eventType)::type;
                if (const auto* realEvent = std::any_cast<TEvent>(&event); realEvent != nullptr) {
                    std::cout << "Found actual event type: " << sml::aux::get_type_name<TEvent>() << std::endl;
                    m_target->process_event(*realEvent);
                    return true;
                }
                return false;
            });
        return wasDispatched;
    }

private:
    Target* m_target;
};

int main()
{
    auto inputProcessor = std::make_unique<InputProcessor>();
    CustomLogger logger;

    using StateMachineType = sml::sm<ExampleStateMachine, sml::logger<CustomLogger>>;

    ActualDispatcher<StateMachineType> dispatcher {};

    StateMachineType sm { logger, static_cast<IInputProcessor&>(*inputProcessor), static_cast<IDispatcher&>(dispatcher) };
    dispatcher.setTarget(sm);

    while (true) {
        std::string line;
        std::getline(std::cin, line);
        const bool result = sm.process_event(events::ReceiveInput { line });

        if (!result) {
            std::cout << "Could not process an event, terminating..." << std::endl;
            return 1;
        }
    }

    return 0;
}
