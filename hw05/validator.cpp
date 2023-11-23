#include "validator.h"

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

bool SqlValidator::is_valid() const {
    return std::holds_alternative<state::Valid>(state_);
}

void SqlValidator::handle(const Token &token) {

    state_ = std::visit(
        [&](auto cur) -> State {
            return transition(cur, token);
        },
        state_
    );
}

struct TransitionFromStartVisitor {
    // Only the `Select` token advances the FSM from the `Start` state
    // TODO: Correct the behaviour
    State operator()(token::Select) const { return state::SelectStmt{}; }

    // All the other tokens, put it in the invalid state
    State operator()(auto) const {return state::Invalid{}; }
};

struct TransitionFromSelectStmtVisitor {
    State operator()(token::Asterisks) const { return state::AllColumns{}; }
    State operator()(token::Identifier) const { return state::NamedColumn{}; }

    State operator()(auto) const {return state::Invalid{}; }
};

struct TransitionFromAllColumns {
    State operator()(token::From) const { return state::FromClause{}; }

    State operator()(auto) const {return state::Invalid{}; }
};

struct TransitionFromNamedColumnVisitor {
    State operator()(token::Comma) const { return state::MoreColumns{}; }
    State operator()(token::From) const { return state::FromClause{}; }

    State operator()(auto) const {return state::Invalid{}; }
};

struct TransitionFromMoreColumnsVisitor {
    State operator()(token::Identifier) const { return state::NamedColumn{}; }

    State operator()(auto) const {return state::Invalid{}; }
};

struct TransitionFromFromClauseVisitor {
    State operator()(token::Identifier) const { return state::TableName{}; }

    State operator()(auto) const {return state::Invalid{}; }
};

struct TransitionFromTableNameVisitor {
    State operator()(token::Semicolon) const { return state::Valid{}; }

    State operator()(auto) const {return state::Invalid{}; }
};

struct TransitionFromValidVisitor {
    State operator()(token::Semicolon) const { return state::Valid{}; }

    State operator()(auto) const {return state::Invalid{}; }
};

struct TransitionFromInvalidVisitor {
    State operator()(auto) const {return state::Invalid{}; }
};

State transition(state::Start, const Token &token) {
    return std::visit(TransitionFromStartVisitor{}, token.value());
}

State transition(state::Valid, const Token &token) {
    return std::visit(TransitionFromValidVisitor{}, token.value());
}

State transition(state::Invalid, const Token &token) {
    return std::visit(TransitionFromInvalidVisitor{}, token.value());
}

State transition(state::SelectStmt, const Token &token) {
    return std::visit(TransitionFromSelectStmtVisitor{}, token.value());
}

State transition(state::AllColumns, const Token &token) {
    return std::visit(TransitionFromAllColumns{}, token.value());
}

State transition(state::NamedColumn, const Token &token) {
    return std::visit(TransitionFromNamedColumnVisitor{}, token.value());
}

State transition(state::MoreColumns, const Token &token) {
    return std::visit(TransitionFromMoreColumnsVisitor{}, token.value());
}

State transition(state::FromClause, const Token &token) {
    return std::visit(TransitionFromFromClauseVisitor{}, token.value());
}

State transition(state::TableName, const Token &token) {
    return std::visit(TransitionFromTableNameVisitor{}, token.value());
}

bool is_valid_sql_query(const std::vector<Token> &tokens) {
    SqlValidator validator;
    for (const auto& token : tokens)
    {
        validator.handle(token);
    }

    return validator.is_valid();
}

} // namespace sql
