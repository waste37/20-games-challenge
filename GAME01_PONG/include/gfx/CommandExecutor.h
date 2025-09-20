#pragma once

namespace Graphics
{

class CommandExecutor {
public:
    CommandExecutor();
    void Execute() const noexcept;
};

} // Graphics
