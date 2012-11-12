
namespace PivotalTracker
{
    class StoryRefreshInfo
    {
        public Story story;
        public bool IsNew;

        public StoryRefreshInfo()
        {
        }

        public StoryRefreshInfo(Story selectedStory, bool isSelectedStoryNew)
        {
            story = selectedStory;
            IsNew = isSelectedStoryNew;
        }
    }
}
