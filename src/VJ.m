function varargout = VJT(varargin)
% VJT M-file for VJT.fig
%      VJT, by itself, creates a new VJT or raises the existing
%      singleton*.
%
%      H = VJT returns the handle to a new VJT or the handle to
%      the existing singleton*.
%
%      VJT('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in VJT.M with the given input arguments.
%
%      VJT('Property','Value',...) creates a new VJT or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before VJT_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to VJT_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help VJT

% Last Modified by GUIDE v2.5 14-Feb-2011 15:19:23

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @VJT_OpeningFcn, ...
                   'gui_OutputFcn',  @VJT_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before VJT is made visible.
function VJT_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to VJT (see VARARGIN)

% Choose default command line output for VJT
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);
% global positivefilename
clc;
load('config');
set(handles.edit7,'String',sprintf('%d',s1.step));
set(handles.edit5,'String',sprintf('%d',s1.Tr_pn));
set(handles.edit4,'String',sprintf('%d',s1.CropValue));
set(handles.edit13,'String',sprintf('%d',s1.subWindowSize));
set(handles.edit8,'String',s1.NegativeDataFile);
set(handles.edit9,'String',s1.PositiveTrDataFile);
set(handles.edit10,'String',s1.PositiveValDataFile);
set(handles.edit11,'String',sprintf('%.2f',s1.TgtFalsePositiveRate));
set(handles.edit12,'String',sprintf('%.2f',s1.TgtCorrectRate));

% UIWAIT makes VJT wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = VJT_OutputFcn(~, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Get default command line output from handles structure

varargout{1} = handles.output;



function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as text
%        str2double(get(hObject,'String')) returns contents of edit2 as a double


% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global positivefilename
global data
global limitnumber
positivefilename = uigetfile({'*.dat','Data files(*.dat)';'*.*','All files'},'Open Positive Data File');
if(positivefilename ~= 0)
    set(handles.edit2,'String',positivefilename);
    [data n_row] = getData(positivefilename);
    set(handles.edit3,'String',sprintf('%d',n_row));
end
limitnumber = n_row/2;
msize = size(data);
subWindowSize = msize(2);
subWindowSize = sqrt(subWindowSize-1);
set(handles.edit13,'String',sprintf('%d',subWindowSize));
set(handles.text8,'String',sprintf('<=%d',limitnumber));
set(handles.edit5,'String',sprintf('%d',limitnumber));
set(handles.edit6,'String',sprintf('%d',limitnumber));
set(handles.pushbutton2,'Enable','on');


function edit3_Callback(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit3 as text
%        str2double(get(hObject,'String')) returns contents of edit3 as a double


% --- Executes during object creation, after setting all properties.
function edit3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit4_Callback(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit4 as text
%        str2double(get(hObject,'String')) returns contents of edit4 as a double


% --- Executes during object creation, after setting all properties.
function edit4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit5_Callback(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit5 as text
%        str2double(get(hObject,'String')) returns contents of edit5 as a double


% --- Executes during object creation, after setting all properties.
function edit5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit6_Callback(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit6 as text
%        str2double(get(hObject,'String')) returns contents of edit6 as a double


% --- Executes during object creation, after setting all properties.
function edit6_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global data
global rowT
global rowV
global step
global Positive_Tr_fname;
global Positive_Val_fname;
global limitnumber
asm = NET.addAssembly('System.Windows.Forms');
import System.Windows.Forms.*;
cropvalue = str2num(get(handles.edit4,'String'));
rowT = str2num(get(handles.edit5,'String'));
rowV = str2num(get(handles.edit6,'String'));
pass = 1;
if(rowT > limitnumber) 
    MessageBox.Show('rowT exceeded');
    pass = 0;
end
if(rowT + rowV > limitnumber * 2)
    MessageBox.Show('Total number exceeded');
    pass = 0;
end
if(rowT < 0 || rowV < 0)
    MessageBox.Show('Value is not in range');
    pass = 0;
end
% disp(pass);

if(pass == 1)
    savePositiveData(data,cropvalue,rowT,rowV);
    Positive_Tr_fname = 'Tr_Positive.dat';
    Positive_Val_fname = 'Val_Positive.dat';
    MessageBox.Show('Positive data saved');
    step = 1;
    set(handles.edit7,'String',sprintf('%d',step));
    set(handles.edit9,'String',Positive_Tr_fname);
    set(handles.edit10,'String',Positive_Val_fname);
end

function edit7_Callback(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit7 as text
%        str2double(get(hObject,'String')) returns contents of edit7 as a double


% --- Executes during object creation, after setting all properties.
function edit7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit8_Callback(hObject, eventdata, handles)
% hObject    handle to edit8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit8 as text
%        str2double(get(hObject,'String')) returns contents of edit8 as a double


% --- Executes during object creation, after setting all properties.
function edit8_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global negativefilename

negativefilename = uigetfile({'*.dat','Data files(*.dat)';'*.*','All files'},'Open Negative Data File');
tempdata = load(negativefilename);
tempsize = size(tempdata);
tempint = tempsize(1);
set(handles.edit14,'String',sprintf('%d',tempint));
tempint = tempsize(2);
tempint = sqrt(tempint-1);
subWindowSize = str2num(get(handles.edit13,'String'));
% asm = NET.addAssembly('System.Windows.Forms');
% import System.Windows.Forms.*;
asm = NET.addAssembly('System.Windows.Forms');
import System.Windows.Forms.*;

if(negativefilename ~= 0)
    if(tempint ~= subWindowSize)
        MessageBox.Show('Data size mismatch!');
    else
%         MessageBox.Show('Data size matches. Go on!');
        set(handles.edit8,'String',negativefilename);
    end
end



function edit9_Callback(hObject, eventdata, handles)
% hObject    handle to edit9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit9 as text
%        str2double(get(hObject,'String')) returns contents of edit9 as a double


% --- Executes during object creation, after setting all properties.
function edit9_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit10_Callback(hObject, eventdata, handles)
% hObject    handle to edit10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double


% --- Executes during object creation, after setting all properties.
function edit10_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit11_Callback(hObject, eventdata, handles)
% hObject    handle to edit11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit11 as text
%        str2double(get(hObject,'String')) returns contents of edit11 as a double


% --- Executes during object creation, after setting all properties.
function edit11_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit12_Callback(hObject, eventdata, handles)
% hObject    handle to edit12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit12 as text
%        str2double(get(hObject,'String')) returns contents of edit12 as a double


% --- Executes during object creation, after setting all properties.
function edit12_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton4.
function pushbutton4_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
step = str2num(get(handles.edit7,'String'));
Tr_pn = str2num(get(handles.edit5,'String'));
CropValue = str2num(get(handles.edit4,'String'));
subWindowSize = str2num(get(handles.edit13,'String'));
NegativeDataFile = get(handles.edit8,'String');
PositiveTrDataFile = get(handles.edit9,'String');
PositiveValDataFile = get(handles.edit10,'String');
TgtFalsePositiveRate = str2double(get(handles.edit11,'String'));
TgtCorrectRate = str2double(get(handles.edit12,'String'));

disp(sprintf('step = %d',step));
disp(sprintf('Tr_pn = %d',Tr_pn));
disp(sprintf('CropValue = %d',CropValue));
disp(sprintf('NegativeDataFile = %s',NegativeDataFile));
disp(sprintf('PositiveTrDataFile = %s',PositiveTrDataFile));
disp(sprintf('PositiveValDataFile = %s',PositiveValDataFile));
disp(sprintf('TgtFalsePositiveRate = %f',TgtFalsePositiveRate));
disp(sprintf('TgtCorrectRate = %f',TgtCorrectRate));

set(handles.edit7,'Enable','inactive');
set(handles.edit5,'Enable','inactive');
set(handles.edit4,'Enable','inactive');
set(handles.edit13,'Enable','inactive');
set(handles.edit8,'Enable','inactive');
set(handles.edit9,'Enable','inactive');
set(handles.edit10,'Enable','inactive');
set(handles.edit11,'Enable','inactive');
set(handles.edit12,'Enable','inactive');
set(handles.pushbutton5,'Enable','inactive');
set(handles.pushbutton6,'Enable','inactive');
set(handles.pushbutton3,'Enable','inactive');

MakeCascade(handles,step,Tr_pn,CropValue,subWindowSize,NegativeDataFile ,PositiveTrDataFile,PositiveValDataFile,TgtFalsePositiveRate,TgtCorrectRate);
disp('activate');
set(handles.edit7,'Enable','on');
set(handles.edit5,'Enable','on');
set(handles.edit4,'Enable','on');
set(handles.edit13,'Enable','on');
set(handles.edit8,'Enable','on');
set(handles.edit9,'Enable','on');
set(handles.edit10,'Enable','on');
set(handles.edit11,'Enable','on');
set(handles.edit12,'Enable','on');
set(handles.pushbutton5,'Enable','on');
set(handles.pushbutton6,'Enable','on');
set(handles.pushbutton3,'Enable','on');
% MakeCascade(1,250,140,24,'S_Negative_Total.dat','Tr_Positive.dat','Val_Positive.dat',50,99)

% --- Executes on button press in pushbutton5.
function pushbutton5_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
filename1 = get(handles.edit10,'String');
filename2 = uigetfile({'*.dat','Data files(*.dat)';'*.*','All files'},'Open Positive Data File');
asm = NET.addAssembly('System.Windows.Forms');
import System.Windows.Forms.*;
if(filename2 ~= 0)
    set(handles.edit9,'String',filename2);
    if(strcmp(filename1,filename2))
        MessageBox.Show('Caution!! Training and Validation positive file name is same.');
    end
end
tempdata = load(filename2);
tempsize = size(tempdata);
tempint = tempsize(1);
set(handles.edit5,'String',sprintf('%d',tempint));
tempint = tempsize(2);
tempint = sqrt(tempint-1);
subWindowSize = str2num(get(handles.edit13,'String'));
set(handles.edit13,'String',sprintf('%d',tempint));
if(subWindowSize ~= tempint)
    MessageBox.Show('Caution!! Data size mismatch!');
end
    
% --- Executes on button press in pushbutton6.
function pushbutton6_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
filename1 = get(handles.edit9,'String');
filename2 = uigetfile({'*.dat','Data files(*.dat)';'*.*','All files'},'Open Positive Data File');
asm = NET.addAssembly('System.Windows.Forms');
import System.Windows.Forms.*;
if(filename2 ~= 0)
    set(handles.edit10,'String',filename2);
    if(strcmp(filename1,filename2))
        MessageBox.Show('Caution!! Training and Validation positive file name is same.');
    end
end
tempdata = load(filename2);
tempsize = size(tempdata);
tempint = tempsize(1);
set(handles.edit6,'String',sprintf('%d',tempint));
tempint = tempsize(2);
tempint = sqrt(tempint-1);
subWindowSize = str2num(get(handles.edit13,'String'));
set(handles.edit13,'String',sprintf('%d',tempint));
if(subWindowSize ~= tempint)
    MessageBox.Show('Caution!! Data size mismatch!');
end

function edit13_Callback(hObject, eventdata, handles)
% hObject    handle to edit13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit13 as text
%        str2double(get(hObject,'String')) returns contents of edit13 as a double


% --- Executes during object creation, after setting all properties.
function edit13_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton7.
function pushbutton7_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see
% GUIDATA)test_function
str_temp = get(handles.edit15,'String');
str_temp = sprintf('%s\n%s','bbb', 'aaa');
set(handles.edit15,'String',str_temp);



function edit14_Callback(hObject, eventdata, handles)
% hObject    handle to edit14 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit14 as text
%        str2double(get(hObject,'String')) returns contents of edit14 as a double


% --- Executes during object creation, after setting all properties.
function edit14_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit14 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton10.
function pushbutton10_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
user_response = modaldlg('Title','Confirm Close');
switch lower(user_response)
    case 'no'
        % take no action
    case 'yes'
        delete(handles.figure1)
        
end


% --- Executes on button press in pushbutton11.
function pushbutton11_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.edit7,'String','');
set(handles.edit5,'String','');
set(handles.edit4,'String','');
set(handles.edit13,'String','');
set(handles.edit8,'String','');
set(handles.edit9,'String','');
set(handles.edit10,'String','');
set(handles.edit11,'String','');
set(handles.edit12,'String','');



function edit15_Callback(hObject, eventdata, handles)
% hObject    handle to edit15 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit15 as text
%        str2double(get(hObject,'String')) returns contents of edit15 as a double


% --- Executes during object creation, after setting all properties.
function edit15_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit15 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit16_Callback(hObject, eventdata, handles)
% hObject    handle to edit16 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit16 as text
%        str2double(get(hObject,'String')) returns contents of edit16 as a double


% --- Executes during object creation, after setting all properties.
function edit16_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit16 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object deletion, before destroying properties.
function figure1_DeleteFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
user_response = modaldlg('Title','Confirm Save','string','Do you want to save settings?');
switch lower(user_response)
case 'no'
	% take no action
case 'yes'
    % Prepare to close GUI application window
	s1.step = str2num(get(handles.edit7,'String'));
    s1.Tr_pn = str2num(get(handles.edit5,'String'));
    s1.CropValue = str2num(get(handles.edit4,'String'));
    s1.subWindowSize = str2num(get(handles.edit13,'String'));
    s1.NegativeDataFile = get(handles.edit8,'String');
    s1.PositiveTrDataFile = get(handles.edit9,'String');
    s1.PositiveValDataFile = get(handles.edit10,'String');
    s1.TgtFalsePositiveRate = str2double(get(handles.edit11,'String'));
    s1.TgtCorrectRate = str2double(get(handles.edit12,'String'));
    save('config','s1');
	delete(handles.figure1)
end

