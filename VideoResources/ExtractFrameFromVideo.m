function run()
    fileName = 'badapple_video.mp4'; 
    obj = VideoReader(fileName);
    num_frames = obj.NumberOfFrames;

    for k = 1: num_frames
    %for k = 200: 200
         frame = read(obj,k);
         frame = rgb2gray(frame);
         frame = imresize(frame,0.5);
         frame = edge(frame,'sobel');
         frame = MakeGreenish(frame);         
         %imshow(frame);
         imwrite(frame,strcat('frames/',num2str(k),'.bmp'),'bmp');
    end
end

function rgb_frame = MakeGreenish(frame)
    rgb_frame = zeros(size(frame,1),size(frame,2),3);
    rgb_frame(:,:,2) = frame;
end
