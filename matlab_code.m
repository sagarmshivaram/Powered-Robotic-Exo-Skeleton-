clear all; %% CLEAR WORKSPACE %%
close all; %% CLOSE ALL THE OPENED FILES %%
clc        %% CLEAR SCREEN %%

if(~isempty(instrfind))  %%CHECK IF SERIAL PORT IS OPEN %%
    fclose(instrfind);   %% CLOSE PORT %%
    delete(instrfind);   %% DELETE SERIAL PORT %%
end

s = serial('COM5','Baudrate',9600); %% CREATE SERIAL PORT OBJECT %%
fopen(s); %% OPEN SERIAL PORT %%

%% GLOBAL VARIABLE TO RECEIVED DATA %%

global iLEGS_Receive
iLEGS_Receive = [];

%% JOINT POSITION STREAMING CONTROL BY REPLAYING REAL RECORDED KINEMATICS DATA %%

pause(2);
InterComandInterval = 0.005/6; % 100ms

%% DISPLAY PARAMETERS %%
%% Y-AXIS RANGE %%
HipRange = [-30, 40];
KneeRange = [-10, 100];
AnkleRange = [-25, 25];
FootSwitchRange = [-0.3,1.3];

%% X-AXIS RANGE IN POINTS %%
PtsRangeLockConstant = 600;

PtsIndex = 1:PtsRangeLockConstant;

%% INITIATING THE REAL TIME WAVEFORM DISPLAY WINDOW %%

figure('unit','normalized','Outerposition',[0 0 1 1]);
set(gcf,'PaperPositionMode','auto');

% Angle Position - RH RK RA

subplot(5,1,1),plot(PtsIndex, zeros(1,PtsRangeLockConstant), 'Color', [0 0 0]);hold on; xlim([1,PtsRangeLockConstant]), ylim([HipRange(1),HipRange(2)]); ylabel('RH Deg');
set(gca,'Color',[0 0 0],'xTickLabel',{[]},'FontSize',16,'FontWeight','bold');

subplot(5,1,2),plot(PtsIndex, zeros(1,PtsRangeLockConstant), 'Color', [0 0 0]);hold on; xlim([1,PtsRangeLockConstant]), ylim([KneeRange(1),KneeRange(2)]); ylabel('RK Deg');
set(gca,'Color',[0 0 0],'xTickLabel',{[]},'FontSize',16,'FontWeight','bold');

subplot(5,1,3),plot(PtsIndex, zeros(1,PtsRangeLockConstant), 'Color', [0 0 0]);hold on; xlim([1,PtsRangeLockConstant]), ylim([AnkleRange(1),AnkleRange(2)]); ylabel('RA Deg');
set(gca,'Color',[0 0 0],'xTickLabel',{[]},'FontSize',16,'FontWeight','bold');

% FootSwitch

subplot(5,1,4),plot(PtsIndex, zeros(1,PtsRangeLockConstant), 'Color', [0 0 0]);hold on; xlim([1,PtsRangeLockConstant]), ylim([FootSwitchRange(1),FootSwitchRange(2)]); ylabel('FS RHeel');
set(gca,'Color',[0 0 0],'xTickLabel',{[]},'FontSize',16,'FontWeight','bold');

subplot(5,1,5),plot(PtsIndex, zeros(1,PtsRangeLockConstant), 'Color', [0 0 0]);hold on; xlim([1,PtsRangeLockConstant]), ylim([FootSwitchRange(1),FootSwitchRange(2)]); ylabel('FS RToe');
set(gca,'Color',[0 0 0],'FontSize',16,'FontWeight','bold');

ReceivedKinematic = [];

%% STORE RECEIVED DATA TO CORRESPONDING ARRAYS %%

RH_Received = [];
RK_Received = [];
RA_Received = [];
FootSwitch_RHeel_Received = [];
FootSwitch_RToe_Received = [];

PIndex = 0;
FrameNum = 3;
disp(FrameNum);

%% REAL TIME PLOTTING LOOP %%

while PIndex < FrameNum * PtsRangeLockConstant
        
    PIndex = PIndex+1
    PIndexInCurrentFrame = mod(PIndex,PtsRangeLockConstant)
    CurrentReceivedDataGram = fscanf(s);
    disp(CurrentReceivedDataGram)
    
        %% RH
           % MotorID = CurrentReceivedDataGram(11:13);
            AnglePosition = CurrentReceivedDataGram(11:14);
            AnglePosition = str2num(AnglePosition);
            AnglePosition = AnglePosition - 1000;
            subplot(5,1,1), plot(PIndexInCurrentFrame,AnglePosition, 'color', [1 1 0], 'LineWidth', 6); xlim([1,PtsRangeLockConstant]), ylim([HipRange(1),HipRange(2)]); ylabel('RH Deg');
            % set(gca,'Color',[0 0 0],'xTickLabel',{[]},'FontSize',16,'FontWeight','bold');
            % Check Figure Frame Update
            if PIndexInCurrentFrame == 0
                hold off;
            else 
                hold on;
            end
            RH_Received = cat(2,RH_Received,AnglePosition);
        %% RK
           % MotorID = CurrentReceivedDataGram(11:13);
            AnglePosition = CurrentReceivedDataGram(21:24);
            AnglePosition = str2num(AnglePosition);
            AnglePosition = AnglePosition - 1000;
            subplot(5,1,2), plot(PIndexInCurrentFrame,AnglePosition, 'color', [1 1 0], 'LineWidth', 6);  xlim([1,PtsRangeLockConstant]), ylim([KneeRange(1),KneeRange(2)]); ylabel('RK Deg');
            % set(gca,'Color',[0 0 0],'xTickLabel',{[]},'FontSize',16,'FontWeight','bold');
            % Check Figure Frame Update
            if PIndexInCurrentFrame == 0
                hold off;
            else 
                hold on;
            end
            RK_Received = cat(2,RK_Received,AnglePosition);
        %% RA
            % MotorID = CurrentReceivedDataGram(11:13);
            AnglePosition = CurrentReceivedDataGram(31:34);
            AnglePosition = str2num(AnglePosition);
            AnglePosition = AnglePosition - 1000;
            subplot(5,1,3), plot(PIndexInCurrentFrame,AnglePosition, 'color', [1 1 0], 'LineWidth', 6); xlim([1,PtsRangeLockConstant]), ylim([AnkleRange(1),AnkleRange(2)]); ylabel('RA Deg');
            % set(gca,'Color',[0 0 0],'xTickLabel',{[]},'FontSize',16,'FontWeight','bold');
            % Check Figure Frame Update
            if PIndexInCurrentFrame == 0
                hold off;
            else 
                hold on;
            end
            RA_Received = cat(2,RA_Received,AnglePosition);
            
        
            
          %% FootSwitch - R Heel
            % MotorID = CurrentReceivedDataGram(11:13);
            FootSwitch_RHeel = CurrentReceivedDataGram(41); %% ?
            FootSwitch_RHeel = str2num(FootSwitch_RHeel);
            subplot(5,1,4), plot(PIndexInCurrentFrame,FootSwitch_RHeel, 'color', [1 0 1], 'LineWidth', 6); xlim([1,PtsRangeLockConstant]), ylim([FootSwitchRange(1),FootSwitchRange(2)]); ylabel('FS RHeel');
            % set(gca,'Color',[0 0 0],'xTickLabel',{[]},'FontSize',16,'FontWeight','bold');
            % Check Figure Frame Update
            if PIndexInCurrentFrame == 0
                hold off;
            else 
                hold on;
            end
            FootSwitch_RHeel_Received = cat(2,FootSwitch_RHeel_Received,AnglePosition);
            
          %% FootSwitch - R Toe
            % MotorID = CurrentReceivedDataGram(11:13);
            FootSwitch_RToe = CurrentReceivedDataGram(48); %% ?
            FootSwitch_RToe = str2num(FootSwitch_RToe);
            subplot(5,1,5), plot(PIndexInCurrentFrame,FootSwitch_RToe, 'color', [1 0 1], 'LineWidth', 6); xlim([1,PtsRangeLockConstant]), ylim([FootSwitchRange(1),FootSwitchRange(2)]); ylabel('FS RToe');
            % set(gca,'Color',[0 0 0],'FontSize',16,'FontWeight','bold');
            % Check Figure Frame Update
            if PIndexInCurrentFrame == 0
                hold off;
            else 
                hold on;
            end
            FootSwitch_RToe_Received = cat(2,FootSwitch_RToe_Received,AnglePosition);
            
            
    pause(InterComandInterval); 
end

%% END OF REAL TIME READING DATA %%

disp('Controlling Mode End')

%% SAVE FINAL DATA TO CORRESPONDING ANGLES %%
save iLEGS_Receive RH_Received RK_Received RA_Received; 
disp('Data save done')

%% CLOSE AND CLEAR SERIAL PORT %%
fclose(s);
delete(s);
clear;
delete(instrfindall);
%% END OF THE FILE %%