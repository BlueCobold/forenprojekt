
namespace PivotalTracker
{
    /// <summary>
    /// What's about Story XYZ?
    /// </summary>
    public enum StoryState
    {
        /// <summary>
        /// The Story isn't started yet.
        /// </summary>
        Unstarted,
        /// <summary>
        /// The Story is started.
        /// </summary>
        Started, 
        /// <summary>
        /// The Story is finished.
        /// </summary>
        Finished,
        /// <summary>
        /// The Story is delivered.
        /// </summary>
        Delivered,
        /// <summary>
        /// The Story is accepted.
        /// </summary>
        Accepted,
        /// <summary>
        /// The Story is rejected.
        /// </summary>
        Rejected
    }
}
