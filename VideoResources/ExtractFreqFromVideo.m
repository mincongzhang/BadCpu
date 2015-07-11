function run()
%http://stackoverflow.com/questions/27670714/video-read-in-matlab-audio-output-port-is-not-relavant
    VidObj=VideoReader('badapple_video.mp4');
    n=VidObj.NumberOfFrames;
    videoFReader = vision.VideoFileReader('badapple_video.mp4');
    [audio,fs]=audioread('badapple_video.mp4');
    op=floor(fs/videoFReader.info.VideoFrameRate);

    for i=298:298  
        videoFrame= step(videoFReader);
        audios=audio( (i-1)*op + 1 : i*op , : );
        %sound(audios,fs);
        wave = getWave(audios);
        
        filename = strcat('frames/a',num2str(i),'.bmp');
        plot(wave);
        print(filename,'-dbmp','-r50');
        I = imread(filename);
        O = cutImage(I);
        imwrite(O,strcat('frames/a',num2str(i),'.bmp'),'bmp');
    end

    release(videoFReader);
end

function O = cutImage(I)
    G = 255 - I(40:size(I,1)-40,60:size(I,2)-60,1);
    O = zeros(size(G,1),size(G,2),3);
    O(:,:,2) = G;
end

function out = getWave(x)
    single_x = x(:,1)+x(:,2);
    x_range = 150;
    out = zeros(x_range,1);
    
    if(abs(sum(single_x)))
        single_x = normToPositive(single_x);
        N = length(single_x);

        interval = floor(N/x_range);
        step = 1; i = 1;
        while(1)
            if (step+interval>N)
                break;
            end

            sample = single_x(step:step+interval);
            out(i,1) = sum(sample)/length(sample);
            step = step+interval;
            i=i+1;
        end
    end
end

function out = normToPositive(x)
    max_x = max(abs(x));
    x = x./max_x;
    x = x.*(180/2);
    out = x+180/2;
end

function frame = getMagFrame(mag)
    frame = zeros(184,153,3);
    for i=1:length(mag)
        if(~isnan(mag(i)))
            frame(ceil(mag(i)+1),i,2) = 255;
        end
    end 
end

function mag_out = getSpectrum(x,fs)

    x = x(:,1);             % get the first channel
    xmax = max(abs(x));     % find the maximum value
    x = x/xmax;             % scalling the signal

    % time & discretisation parameters
    N = length(x);
    t = (0:N-1)/fs;       

    % spectral analysis
    win = hanning(N);       % window
    K = sum(win)/N;         % coherent amplification of the window
    X = abs(fft(x.*win))/N; % FFT of the windowed signal
    NUP = ceil((N+1)/2);    % calculate the number of unique points
    X = X(1:NUP);           % FFT is symmetric, throw away second half 
    if rem(N, 2)            % odd nfft excludes Nyquist point
        X(2:end) = X(2:end)*2;
    else                    % even nfft includes Nyquist point
        X(2:end-1) = X(2:end-1)*2;
    end
    f = (0:NUP-1)*fs/N;     % frequency vector
    X = 20*log10(X);        % spectrum magnitude

    mag = X - min(X);
    %freq range 0 - 150
    freq_range = 150;
    mag_out = zeros(freq_range,1);
    interval = floor((N/freq_range)/2);
    step = 1;
    i = 1;
    while(1)
        if (step+interval>length(mag)/2) 
            break; 
        end

        sample = mag(step:step+interval);
        mag_out(i,1) = sum(sample)/length(sample);
        step = step+interval;
        i=i+1;
    end
end