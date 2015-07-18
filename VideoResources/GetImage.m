function run()
    frame = imread('m.jpeg');
    frame = rgb2gray(frame);
    frame = imresize(frame,0.5);
    frame = edge(frame,'sobel');
    frame = MakeGreenish(frame);         
    imshow(frame);
    imwrite(frame,strcat('m.bmp'),'bmp');
end

function rgb_frame = MakeGreenish(frame)
    rgb_frame = zeros(size(frame,1),size(frame,2),3);
    rgb_frame(:,:,2) = frame;
end
