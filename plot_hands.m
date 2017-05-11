%% input: struct array frame with individual hand locations
% % assuming that variable 'frame' is present in the data.
% load testframe.mat % turn this line on to load data from testframe.mat file

%% plot the hand position over time

% maak een figuur aan; zorg dat het figuur leeg is(clf); zorg dat als je > 2 
% plotjes maakt, ook de 1ste in het figuur blijft staan
fig = figure(1); clf; hold on

% bereken hoe groot je figuur moet worden aan de hand van de positie van de
% 'pointables' (de vinger toppen) en de arm positie
all_pointables = cell2mat(arrayfun(@(x) x.pointables,frame,'uniformoutput',false).');
arm_position = cell2mat(arrayfun(@(f) f.hands.arm_elbowPosition, frame,'uniformoutput',false).');
% calculate min and max values
boundaries = [min([cell2mat({all_pointables.position}.');arm_position]);...
    max([cell2mat({all_pointables.position}.');arm_position])];
% set the axis to the boudaries
axis(boundaries(:).')
% show the axis in 3d using axis vis3d and view(3)
axis vis3d
view(3);

previous_frame_id = 0; % keep track of frame id

% loop over all frames
for current_frame = 1:length(frame)
    if frame(current_frame).id ~= previous_frame_id % ONLY show the next frame if the frame id is updated.
        % if the frame_id is the same, the data inside this frame is also
        % the same. SMarter would be if this is already checked when a frame is
        % saved. I.e. only save a frame if it is a new frame.
        
        previous_frame_id = frame(current_frame).id;
        
        % set the title of the window to the frame and include the time in
        % milliseconds
        title(sprintf('frame %i of %i (time: %0.2f milleseconds since start)',current_frame,length(frame),(frame(current_frame).timestamp-frame(1).timestamp)/1000))
        set(fig,'name',sprintf('frame %i of %i (time: %0.2f milleseconds since start)',current_frame,length(frame),(frame(current_frame).timestamp-frame(1).timestamp)/1000));
        
        % plot all pointables (=finger tips or pen tips etc.)
        for current_pointable = 1:length(frame(current_frame).pointables)
            % get position, direction
            position = frame(current_frame).pointables(current_pointable).position;
            direction = frame(current_frame).pointables(current_pointable).direction;
            
            % plot the direction as an arrow (scaled 5x)
            direction = direction * 5; % scaling
            quiver3(position(1),position(2),position(3),...
                direction(1),direction(2),direction(3),'r.-');
            % plot the position
            plot3(position(1),position(2),position(3),'r.');
        end
        
        % now for each frame plot all hand bones from all finger that come from all hands
        % a frame has multiple hands
        %    a hand has some position properties (wrist postiion, arm position,
        %                                         palm position, sphere center etc.)
        %    a hand has multiple fingers
        %        a finger has multiple bones
        %             plot these bones one by one
        for current_hand = 1:length(frame(current_frame).hands)
            
            % get all hand related positions
            palm_position = frame(current_frame).hands(current_hand).palm_position;
            wrist_position = frame(current_frame).hands(current_hand).wrist_position;
            arm_elbowPosition = frame(current_frame).hands(current_hand).arm_elbowPosition;
            arm_wristPosition = frame(current_frame).hands(current_hand).arm_wristPosition;
            sphere_center = frame(current_frame).hands(current_hand).sphere_center;
            
            % plot all hand related positions as points
            plot3(palm_position(1),palm_position(2),palm_position(3),'o')
            plot3(wrist_position(1),wrist_position(2),wrist_position(3),'o')
            plot3(arm_elbowPosition(1),arm_elbowPosition(2),arm_elbowPosition(3),'o')
            plot3(arm_wristPosition(1),arm_wristPosition(2),arm_wristPosition(3),'o')
            plot3(sphere_center(1),sphere_center(2),sphere_center(3),'o')
            
            % loop over all fingers from the current hand (this is done for
            % every hand again)
            for current_finger = 1:length(frame(current_frame).hands(current_hand).fingers)
                % get the current finger
                finger = frame(current_frame).hands(current_hand).fingers(current_finger);
                
                if isempty(finger.finger_type)
                    % no finger is detected; the finger will not be plotted!
                else
                    % loop over all bones of the current finger
                    for current_finger_bone = 1:length(finger.bones)
                        % get position as length/2 and -length/2 relative to the center in
                        % the direction of the 'finger.direction'
                        finger_from = finger.bones(current_finger_bone).center - finger.bones(current_finger_bone).length/2 * finger.bones(current_finger_bone).direction;
                        finger_to = finger.bones(current_finger_bone).center + finger.bones(current_finger_bone).length/2 * finger.bones(current_finger_bone).direction;
                        finger_location = [finger_to; finger_from];
                        
                        % plot the finger as a green line with dots (o) at the end
                        plot3(finger_location(:,1).',finger_location(:,2).',finger_location(:,3).','go-')
                    end
                end
            end
            
        end
        
        % use drawnow here makes sure that every frame is shown on the screen
        % as fast as possible. If you do not use drawnow, the figure will not
        % update every time, but only the last frame (when the code is
        % finished)
        drawnow; % plot every frame
        
        % we do not want to see hands, fingers from the previous frame.
        % delete all items in the figure / axis before going to the next frame
        if current_frame ~= length(frame)
            cla % clear all items from the axis.
        end
    end
end