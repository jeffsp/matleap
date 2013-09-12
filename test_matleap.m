% @file test_matleap.m
% @brief test matleap functionality
% @author Jeff Perry <jeffsp@gmail.com>
% @version 1.0
% @date 2013-09-12

function test_matleap
    matleap('cb',1)
end

function cb(x)
    disp 'callback'
end
