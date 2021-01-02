#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    addAndMakeVisible (frequencySlider);
    frequencySlider.setRange (50.0, 5000.0);
    frequencySlider.setSkewFactorFromMidPoint (500.0); // [4]
   
    addAndMakeVisible (wavefoldSlider);
    wavefoldSlider.setRange (1., 8.);
    //wavefoldSlider.setSkewFactorFromMidPoint (500.0); // [4]
 
    

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent:: updateAngle(){
    
    auto cyclesPerS = frequencySlider.getValue() / csampleRate;
    angleDelta = cyclesPerS * 2 * juce::MathConstants<double>::pi;
    
  

    
    
}
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    csampleRate = sampleRate;
    updateAngle();
    
    frequencySlider.onValueChange = [this]{
        if (csampleRate > 0){
            updateAngle();
        }
    };
    
   
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto level = .2;
    auto* leftB = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightB = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    for (auto samples = 0; samples < bufferToFill.numSamples; samples++){
        auto currentSample = (float)std::sin(currentAngle);
       
        currentSample *= wavefoldSlider.getValue();
        
        if (currentSample > 1){
              currentSample = 2-currentSample;
        }
        
        if (currentSample < -1){
                   currentSample = -2-currentSample;
             }
        if (currentSample > 1){
                 currentSample = 2-currentSample;
           }
           
        if (currentSample < -1){
                      currentSample = -2-currentSample;
                }
        
        currentAngle+=angleDelta;
        
        leftB[samples] = currentSample * level;
        rightB[samples] = currentSample * level;
        
        
    }
    
   
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
        auto sliderLeft = 10;
        frequencySlider.setBounds (sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
        wavefoldSlider.setBounds (sliderLeft, 40, getWidth() - sliderLeft - 10, 40);
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
