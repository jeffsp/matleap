% @file build.m
% @brief build the matleap mex module
% @author Jeff Perry <jeffsp@gmail.com>
% @version 1.0
% @date 2013-09-12

% check for sdk
if exist('LeapSDK')~=7
    error('The "LeapSDK" folder cannot be found.  A link with this name should point to the directory that contains the SDK.');
end

% determine where the Leap library is located
if ismac
    libdir_switch='-L./LeapSDK/lib';
elseif isunix || ispc
    % check register size
    if findstr('64',mexext)
        libdir_switch='-L./LeapSDK/lib/x64';
    else
        libdir_switch='-L./LeapSDK/lib/x86';
    end
else
    error('Unknown operating system');
end

% create the mex command line
fn='matleap.cpp';
fprintf('Compiling %s\n',fn);
cmd=['mex',...
    ' -I./LeapSDK/include ',...
    libdir_switch,...
    ' -lLeap ',...
    fn];
fprintf('Evaluating "%s"\n',cmd)

% run mex
eval(cmd)

fprintf('Done\n')
