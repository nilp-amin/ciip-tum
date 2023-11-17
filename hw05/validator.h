#pragma once

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

namespace state {
/// Initial state of the FSM
struct Start {};
/// State if any transition was wrong, no matter the token, the FSM will stay in the invalid state
struct Invalid {};
/// If the sequence of tokens is correct the FSM are in the valid state, wrong tokens put the FSM
/// into the invalid state, only the semicolon is allowed.
struct Valid {};

struct SelectStmt {};
struct AllColumns {};
struct NamedColumn {};
struct MoreColumns {};
struct FromClause {};
struct TableName {};

} // namespace state

/// variant of all possible states of our finite machine
using State =
    std::variant<state::Start,
                 state::Invalid,
                 state::Valid,
                 state::SelectStmt,
                 state::AllColumns,
                 state::NamedColumn,
                 state::MoreColumns,
                 state::FromClause,
                 state::TableName>;

/// Transition from the `Start` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Start, const Token &token);

/// Transition from the `Valid` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Valid, const Token &token);

/// Transition from the `Invalid` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Invalid, const Token &token);

// TODO: all of the transition functions from the newly created states go
// between here...
// ... and here
[[nodiscard]]
State transition(state::SelectStmt, const Token &token);

[[nodiscard]]
State transition(state::AllColumns, const Token &token);

[[nodiscard]]
State transition(state::NamedColumn, const Token &token);

[[nodiscard]]
State transition(state::MoreColumns, const Token &token);

[[nodiscard]]
State transition(state::FromClause, const Token &token);

[[nodiscard]]
State transition(state::TableName, const Token &token);

/// Our finite state machine.
/// The initial state is `Start` and based on the given tokens it will move to
/// the next state. You can query if the given sequence of tokens was valid with
/// the `is_valid` member function.
class SqlValidator {
public:
    SqlValidator() = default;

    /// Returns `true` iff the variant is in the `Valid` state
    [[nodiscard]]
    bool is_valid() const;

    /// Moves from one state to the next given the token.
    void handle(const Token &token);

private:
    State state_ = state::Start{};
};

[[nodiscard]]
bool is_valid_sql_query(const std::vector<Token> &tokens);
} // namespace sql
